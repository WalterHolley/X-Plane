//Copyright 2024 Walter Holley.  All Rights Reserved

#ifndef MQCLIENT_H
#define MQCLIENT_H

#include<string>

#ifdef IBM
#include "windows.h"
#include "mq.h"
#include "tchar.h"
#endif


class MQClient {

public:
    long init();
    bool send(std::string message);
    void receive();
    void close();



};


#endif //_MQCLIENT_H
