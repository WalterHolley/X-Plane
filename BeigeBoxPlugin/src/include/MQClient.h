// Copyright 2024 Walter Holley.  All Rights Reserved

#ifndef MQCLIENT_H
#define MQCLIENT_H

#include "Logger.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <vector>

using namespace boost::interprocess;
struct bbmsg {
  unsigned int msgType;
  char message[256];
};

enum bbMsgType {
  ERROR = 0,
  BUTTON1 = 1,
  BUTTON2 = 2,
  BUTTON3 = 3,
  INIT = 4,
  RUN = 5,
  END = 6,
  FAIL = 7,
  RESET = 8,
  PING = 9,
  GETSIMSOCKET = 10,
  GETSERVERSOCKET = 11,
  MESSAGE = 12,
  BUFFERSIZEERROR = 13,
  BUFFERSTRINGERROR = 14,
  PLANNINGFAIL = 15
};

class MQClient {

public:
  MQClient(Logger *log);
  bool init();
  bool send(bbmsg &message);
  std::vector<bbmsg> receive();
  void close();

private:
  Logger *_log;
  bool mqInited;
};

#endif //_MQCLIENT_H
