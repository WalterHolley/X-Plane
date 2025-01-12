

#include <fcntl.h>
#include <string>
#define LISTENER_QUEUE "cwiq_mq_post"
#define REPLY_QUEUE "cwiq_mq_reply"

#include "include/MQClient.h"
#include <mqueue.h>

mqd_t listenerQueue;
mqd_t replyQueue;
mq_attr attribs;

MQClient::MQClient(Logger *log) {
  _log = log;
  mqInited = false;
}

bool MQClient::init() {
  if (!mqInited) {
    attribs.mq_maxmsg = 100;
    attribs.mq_msgsize = sizeof(bbmsg);
    attribs.mq_flags = 0;
    attribs.mq_curmsgs = 0;

    listenerQueue =
        mq_open(LISTENER_QUEUE, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attribs);
    replyQueue =
        mq_open(REPLY_QUEUE, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attribs);

    if (listenerQueue == (mqd_t)-1 || replyQueue == (mqd_t)-1) {
      _log->error("MQCLIENT: MQs failed to initialize");

    } else {
      mqInited = true;
    }
  }

  return mqInited;
}

bool MQClient::send(bbmsg message) {
  bool sent = false;
  if (mqInited) {
    auto result = mq_send(replyQueue, (const char *)&message, sizeof(bbmsg), 0);
    if (result == 0) {
      sent = true;
    } else {
      _log->error("MQCLIENT: Could not place message on lister MQ: " +
                  std::to_string(result));
    }
  }
  return sent;
}
