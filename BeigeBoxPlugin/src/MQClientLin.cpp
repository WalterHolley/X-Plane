

#include "include/MQClient.h"
#include <cstdio>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
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
    attribs.mq_maxmsg = 20;
    attribs.mq_msgsize = MSG_SIZE;
    attribs.mq_flags = 0;
    attribs.mq_curmsgs = 0;

    listenerQueue = mq_open(LISTENER_QUEUE, O_CREAT | O_RDONLY | O_NONBLOCK,
                            0666, &attribs);
    _log->debug("MQCLIENT: Message size: " +
                std::to_string(attribs.mq_msgsize));
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
    _log->debug("MQCLIENT:  Outgoing message size: " +
                std::to_string(sizeof(message)));
    int result;
    result = 0;
    result = mq_send(replyQueue, (const char *)&message, sizeof(message), 0);
    _log->debug("MQCLIENT:  Message Sent: " + std::to_string(message.msgType) +
                "|" + std::string(message.message));

    if (result == 0) {
      sent = true;
    } else {
      result = errno;
      _log->error("MQCLIENT:  Could not send message: " +
                  std::string((const char *)strerror(errno)));
    }
  }
  return sent;
}
std::vector<bbmsg> MQClient::receive() {
  std::vector<bbmsg> messages = {};
  ssize_t size;
  u_int8_t msg[sizeof(bbmsg)];
  int count = 0;
  if (mqInited) {
    do {
      bbmsg result;
      u_int8_t msg[MSG_SIZE];
      size = mq_receive(listenerQueue, (char *)&msg, MSG_SIZE, 0);
      if (size < 0 || size > MSG_SIZE) {
        _log->error("MQCLIENT: Failed to receive message: " +
                    std::string((const char *)strerror(errno)));
        _log->error("MQCLIENT: Failed message size: " + std::to_string(size));
        break;
      }
      result.msgType = static_cast<unsigned int>(msg[0]);
      for (int i = 4; i < sizeof(msg); i++) {
        result.message[i - 4] = (char)msg[i];
      }
      _log->debug(
          "MQCLIENT: Message Received: " + std::to_string(result.msgType) +
          "|" + std::string(result.message));
      _log->debug("MQCLIENT: Received message size: " + std::to_string(size));
      messages.push_back(result);
      count++;
    } while (size > 0 && count < 10);
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
