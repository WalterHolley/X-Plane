//
// Created by Zero on 9/27/2024.
//

#include "include/MQClient.h"
#include <vector>
#define BUFFER_LENGTH 256
#define LISTENER_QUEUE_NAME "cwiq_mq_post"
#define REPLY_QUEUE_NAME "cwiq_mq_reply"

MQClient::MQClient(Logger *log) {
  _log = log;
  mqInited = false;
}

bool MQClient::init() {
  if (!mqInited) {
    BOOST_TRY {
      message_queue::remove(LISTENER_QUEUE_NAME);
      message_queue::remove(REPLY_QUEUE_NAME);

      message_queue listen_mq(create_only, LISTENER_QUEUE_NAME, 100,
                              sizeof(bbmsg));
      _log->info("MQCLIENT: Message queue inited: " +
                 std::string(LISTENER_QUEUE_NAME));
      message_queue reply_mq(create_only, REPLY_QUEUE_NAME, 100, sizeof(bbmsg));
      _log->info("MQCLIENT: MEssage queue inited: " +
                 std::string(REPLY_QUEUE_NAME));
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

bool MQClient::send(bbmsg &mqMessage) {
  bool result = false;
  if (mqInited) {
    message_queue mq(open_only, REPLY_QUEUE_NAME);
    mq.send(&mqMessage, sizeof(mqMessage), 0);
    result = true;
    std::string msg(mqMessage.message);
    _log->debug("MQCLIENT: Message sent: " + msg);
  }

  return result;
}

std::vector<bbmsg> MQClient::receive() {
  message_queue mq(open_only, LISTENER_QUEUE_NAME);
  message_queue::size_type msgSize;
  std::vector<bbmsg> processRequests = {};
  unsigned int priority;
  bool gotMsg = false;

  do {
    bbmsg mqMessage;
    gotMsg = mq.try_receive(&mqMessage, sizeof(bbmsg), msgSize, priority);

    if (gotMsg) {
      processRequests.push_back(mqMessage);
    }
  } while (gotMsg);

  return processRequests;
}

void MQClient::close() {
  message_queue::remove(LISTENER_QUEUE_NAME);
  message_queue::remove(REPLY_QUEUE_NAME);
  mqInited = false;
  _log->debug("MQCLIENT: MQ closed");
}
