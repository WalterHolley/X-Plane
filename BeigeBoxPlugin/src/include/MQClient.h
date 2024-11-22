// Copyright 2024 Walter Holley.  All Rights Reserved

#ifndef MQCLIENT_H
#define MQCLIENT_H

#include "Logger.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <vector>

using namespace boost::interprocess;
struct bbmsg {
  long msgType;
  char message[256];
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
