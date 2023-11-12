
#ifndef DATAUTIL_H
#define DATAUTIL_H

#include "../SDK/CHeaders/XPLM/XPLMUtilities.h"
#include "../boost/asio.hpp"
#include "c_headers.h"
#include <string>
#include <cstring>
#include <iostream>
#include <time.h>

#define MAX_CWIC_BUFFER 10000
#define MAX_TIME 20
#define REMOTE_PORT 49042 //TODO:  Bad place for configuration. change later
using namespace std;
using boost::asio::ip::udp;
using boost::asio::ip::address;

class DataUtil {
public:
    DataUtil();
    bool send(string message);
    string receive();
    void writeLog(string message);
    ~DataUtil();
private:
    char* outFilePath;
    FILE* logFile;
    void* cwic;
    int8_t* cwic_socket_addr;
    char* cwic_buffer;
    udp::endpoint _endpoint;
    time_t t;
};

#endif //DATAUTIL_H
