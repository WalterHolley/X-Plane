
#ifndef DATAUTIL_H
#define DATAUTIL_H

#include "../SDK/CHeaders/XPLM/XPLMUtilities.h"
#include "../boost/asio.hpp"
#include "../boost/json/src.hpp"
#include "../boost/array.hpp"
#include "c_headers.h"
#include <string>
#include <cstring>
#include <iostream>
#include <time.h>

#define MAX_CWIC_BUFFER 10000
#define MAX_TIME 20
#define REMOTE_PORT 49042 //TODO:  Bad place for configuration. change later
namespace  json = boost::json;
using namespace std;

using boost::asio::ip::udp;
using boost::asio::ip::address;

const json::string_view  STATES_KEY = "state";
const json::string_view INPUTS_KEY = "input";
const json::string_view INSTRUCTIONS_KEY = "instructions";
const json::string_view FAILURES_KEY = "failures";

struct dataStruct {
    string dref;
    string description;
    int index;
    string units;
    int freq;
    int rounding;
    string value;

};

struct dataFrame{
    vector<dataStruct> state;
    vector<dataStruct> inputs;
    vector<dataStruct> instructions;
    vector<dataStruct> failures;
};

struct dataReference {
    XPLMDataRef dataref;
    string type;
};

class DataUtil {
public:
    DataUtil();
    bool send(dataFrame message);
    dataFrame receive();
    void writeLog(string message);
    ~DataUtil();
private:
    char* outFilePath;
    FILE* logFile;
    boost::array<char, MAX_CWIC_BUFFER> buffer;
    void* cwic;
    int8_t* cwic_socket_addr;
    char* cwic_buffer;
    udp::endpoint _endpoint;
    time_t t;
};

#endif //DATAUTIL_H
