

#include <cstdio>
#include <errno.h>
#include <exception>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "include/MQClient.h"
#include <mqueue.h>
#include <vector>

mqd_t listenerQueue;
mqd_t replyQueue;
mq_attr attribs;

const char *LISTENER_QUEUE = "/cwic_mq_post";
const char *REPLY_QUEUE = "/cwic_mq_reply";

MQClient::MQClient(Logger *log) {
  _log = log;
  mqInited = false;
}

bool MQClient::init() {
  if (!mqInited) {
    attribs.mq_maxmsg = 10;
    attribs.mq_msgsize = sizeof(bbmsg);
    attribs.mq_flags = 0;
    attribs.mq_curmsgs = 0;

    listenerQueue = mq_open(LISTENER_QUEUE, O_CREAT | O_RDONLY | O_NONBLOCK,
                            0666, &attribs);

    mqInited = true;
    if (listenerQueue == (mqd_t)-1) {

      _log->error("MQCLIENT: Listener MQ failed to initialize: " +
                  std::to_string(errno));
      mqInited = false;
    } else {
      replyQueue =
          mq_open(REPLY_QUEUE, O_CREAT | O_WRONLY | O_NONBLOCK, 0666, &attribs);
      if (replyQueue == (mqd_t)-1) {
        _log->error("MQCLIENT: Reply MQ failed to initialize: " +
                    std::to_string(errno));
        mqInited = false;
      }
    }
  }

  return mqInited;
}

bool MQClient::send(bbmsg message) {
  bool sent = false;
  if (mqInited) {
    char msg[256];
    int result;
    sprintf(msg, "%d|%s", message.msgType, message.message);
    _log->debug(msg);
    try {
      result = mq_send(replyQueue, msg, sizeof(msg), 0);

    } catch (std::exception ex) {
      result = errno;
    }
    if (result == 0) {
      sent = true;
    } else {
      _log->error("MQCLIENT: Could not send message: " +
                  std::to_string(result));
    }
  }
  return sent;
}
std::vector<bbmsg> MQClient::receive() {
  std::vector<bbmsg> messages = {};
  ssize_t size;
  int count = 0;
  if (mqInited) {
    do {
      bbmsg msg;
      size = mq_receive(listenerQueue, (char *)&msg, sizeof(bbmsg), 0);

      if (size <= 0) {
        break;
      }
      messages.push_back(msg);
      count++;
    } while (count < 10);
  }

  return messages;
}

void MQClient::close() {
  mq_close(listenerQueue);
  mq_close(replyQueue);
  mq_unlink(LISTENER_QUEUE);
  mq_unlink(REPLY_QUEUE);
  _log->info("MQCLIENT: queues closed");
  mqInited = false;
}
