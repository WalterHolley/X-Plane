// Copyright (c) 2024 Walter Holley III. All Rights Reserved.

#include "include/MQClient.h"
#include "boost/interprocess/creation_tags.hpp"
#include "boost/interprocess/interprocess_fwd.hpp"
#include "boost/interprocess/permissions.hpp"
#include <vector>
#define BUFFER_LENGTH 256
#define LISTENER_QUEUE_NAME "cwiq_mq_post"
#define REPLY_QUEUE_NAME "cwiq_mq_reply"

MQClient::MQClient(Logger *log) {
  _log = log;
  mqInited = false;
  _perms = permissions();
}

bool MQClient::init() {
  if (!mqInited) {
    BOOST_TRY {
      message_queue::remove(LISTENER_QUEUE_NAME);
      message_queue::remove(REPLY_QUEUE_NAME);

      mqInited = true;
    }
    BOOST_CATCH(interprocess_exception & ex) {
      _log->error("MQCLIENT: Could not initialize MQ client: " +
                  std::string(ex.what()));
    }
    BOOST_CATCH_END
  }

  return mqInited;
}

bool MQClient::send(bbmsg mqMessage) {
  bool result = false;
  if (mqInited) {
    try {
      _perms.set_unrestricted();
      message_queue mq(open_or_create, REPLY_QUEUE_NAME, 100,
                       sizeof(struct bbmsg), _perms);
      std::string msg(mqMessage.message);
      _log->debug("MQCLIENT: Sending message: " + msg);
      result = mq.try_send(&mqMessage, sizeof(struct bbmsg), 0);
      if (result) {
        _log->debug("MQCLIENT: Message sent");
      }
    } catch (interprocess_exception &e) {
      _log->error(e.what());
      message_queue::remove(LISTENER_QUEUE_NAME);
      message_queue::remove(REPLY_QUEUE_NAME);
    }
  }

  return result;
}

std::vector<bbmsg> MQClient::receive() {
  _perms.set_unrestricted();
  message_queue mq(open_or_create, LISTENER_QUEUE_NAME, 100,
                   sizeof(struct bbmsg), _perms);
  message_queue::size_type msgSize;
  std::vector<bbmsg> processRequests = {};
  unsigned int priority;
  bool gotMsg = false;
  int count = 0;
  do {
    bbmsg mqMessage;
    gotMsg =
        mq.try_receive(&mqMessage, sizeof(struct bbmsg), msgSize, priority);

    if (gotMsg) {
      processRequests.push_back(mqMessage);
      count++;
    }
  } while (gotMsg && count < 100);

  return processRequests;
}

void MQClient::close() {
  message_queue::remove(LISTENER_QUEUE_NAME);
  message_queue::remove(REPLY_QUEUE_NAME);
  mqInited = false;
  _log->debug("MQCLIENT: MQ closed");
}
