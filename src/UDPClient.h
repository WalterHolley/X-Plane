//
// Created by zero on 1/4/24.
//

#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#define MAX_CWIC_BUFFER 10000

#define REMOTE_PORT 49042 //TODO:  Bad place for configuration. change later
#ifdef IBM
#include<winsock2.h>
#include<windows.h>
#endif

#include "Logger.h"
#include "DataUtil.h"
#include "../SDK/cwic/include/c_headers.h"


class UDPClient
{
public:
    UDPClient(Logger* log);
    void open();
    bool connectionIsOpen();
    bool send(dataFrame df);
    dataFrame receive();
    ~UDPClient();
    void close();
private:
    void* cwic;
    int8_t* cwic_socket_addr;
    char* cwic_buffer;
    DataUtil* _util;
    Logger* _log;
    bool _connectionIsOpen;

};

#endif //UDPCLIENT_H

