

#include "DataUtil.h"


boost::system::error_code ec;
boost::asio::io_service io_service;
udp::socket _socket(io_service);

void msgWait()
{

}

DataUtil::DataUtil()
{
    //logging setup
    XPLMGetSystemPath(outFilePath);
    strcat(outFilePath, "TestValues.txt");
    logFile = fopen(outFilePath, "w");

    //CWIC resource init
    cwic = cwic_init();
    cwic_socket_addr = (int8_t*)malloc(sizeof(int8_t));
    cwic_buffer = (char*)malloc(MAX_CWIC_BUFFER*sizeof(char));
    _endpoint = udp::endpoint(address::from_string(cwic_buffer), REMOTE_PORT);

    //UDP connection setup
    cwic_cwic_sock_addr(cwic, cwic_socket_addr);

}

bool DataUtil::send(string message)
{
    bool result = false;
    try
    {
        _socket.open(udp::v4());
        _socket.send_to(boost::asio::buffer(message), _endpoint);
        _socket.close();
        result = true;
    }
    catch(exception &ex)
    {

    }
    return result;
}

string DataUtil::receive()
{
    string result;


    try
    {
        _socket.open(udp::v4());
        _socket.bind(_endpoint, ec);

        if(ec)//handle socket error
        {

        }
        else
        {

        }
    }
    catch(exception &ex)
    {

    }

    return result;
}

void DataUtil::writeLog(string message)
{
    char time_string[MAX_TIME];
    strftime(time_string, MAX_TIME, "%mm-%dd-%yyyy", localtime(&t));
    fprintf(logFile, "%s: %s\n", time_string, message.c_str());
}

DataUtil::~DataUtil()
{
    fclose(logFile);
    _socket.close();
    free(cwic);
    if(cwic_socket_addr)
        free(cwic_socket_addr);
    if(cwic_buffer)
        free(cwic_buffer);

}