//Copyright 2024 Walter Holley.  All Rights Reserved

#ifndef MQCLIENT_H
#define MQCLIENT_H

#include<string>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "Logger.h"

using namespace  boost::interprocess;

class MQClient {

public:
    MQClient(Logger* log);
    bool init();
    bool send(std::string message);
    void receive();
    void close();
private:
    Logger* _log;
    bool mqInited;




};


#endif //_MQCLIENT_H
