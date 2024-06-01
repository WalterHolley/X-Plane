//
// Created by zero on 1/4/24.
//

#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#define MAX_CWIC_BUFFER 10000

#define REMOTE_PORT 49042 //TODO:  Bad place for configuration. change later

#ifdef WIN32
#include<winsock2.h>
#include<windows.h>
#endif
#include "DataUtil.h"
#include "../SDK/cwic/include/c_headers.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace std;
using boost::asio::ip::udp;
using boost::asio::ip::address;

class UDPClient
{
public:
    UDPClient();
    void open();
    bool send(dataFrame df);
    dataFrame receive();
    ~UDPClient();
    void close();
private:
    DataUtil* _util;

    boost::array<char, MAX_CWIC_BUFFER> buffer;
    void* cwic;
    int8_t* cwic_socket_addr;
    char* cwic_buffer;
    udp::endpoint _endpoint;
};

#endif //UDPCLIENT_H

