//
// Created by zero on 1/4/24.
//

#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#define MAX_CWIC_BUFFER 10000

#define REMOTE_PORT 49042 //TODO:  Bad place for configuration. change later


#include "DataUtil.h"
#include "../SDK/cwic/include/c_headers.h"


using namespace std;

class UDPClient
{
public:
    UDPClient(DataUtil *util);
    void open();
    bool send(dataFrame df);
    dataFrame receive();
    ~UDPClient();
    void close();
private:
    DataUtil* _util;
    void* cwic;
    int8_t* cwic_socket_addr;
    char* cwic_buffer;

};

#endif //UDPCLIENT_H

