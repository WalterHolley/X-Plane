//
// Created by zero on 1/5/24.
//
#ifdef IBM
#include<winsock2.h>
#include<windows.h>
#endif

#include "UDPClient.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::address;
boost::asio::io_service io_service;
udp::socket _socket(io_service);
udp::endpoint _endpoint;

boost::array<char, MAX_CWIC_BUFFER> buffer;

//*****PUBLIC METHODS*****//
UDPClient::UDPClient()
{
    //CWIC resource init
    cwic = cwic_init();
    cwic_socket_addr = (int8_t*)malloc(sizeof(int8_t));
    cwic_buffer = (char*)malloc(MAX_CWIC_BUFFER*sizeof(char));
    _util = new DataUtil();

    //UDP connection setup
    cwic_cwic_sock_addr(cwic, cwic_socket_addr);
    memcpy(cwic_buffer, &cwic_socket_addr, sizeof(cwic_socket_addr));

}

void UDPClient::open()
{
    _endpoint = udp::endpoint(address::from_string(cwic_buffer), REMOTE_PORT);
    _socket.open(udp::v4());
}

void UDPClient::close()
{
    if(_socket.is_open())
    {
        _socket.close();
    }
}

bool UDPClient::send(dataFrame df)
{
    bool result = false;
    try
    {
        string message = _util->dataframeToString(df);
        _socket.send_to(boost::asio::buffer(message), _endpoint);
        result = true;
    }
    catch(std::exception &ex)
    {
        string msg = "There was a problem sending the data to cwic: ";
        msg.append(ex.what());

        _util->writeToLog(msg);

    }
    return result;
}

dataFrame UDPClient::receive()
{

    dataFrame returnValue;

    try
    {
        if(_socket.is_open()){
            fill(buffer.begin(), buffer.end(), NULL);
            _socket.receive(boost::asio::buffer(buffer));
            returnValue = _util->getScenarioData(buffer.c_array());
        }
        else{
            _util->writeToLog("Data connection is not available.");
        }
    }
    catch(std::exception &ex)
    {
        _socket.close();
    }

    return returnValue;
}

UDPClient::~UDPClient()
{
    _socket.close();
    free(cwic);
    free(_util);
    if(cwic_socket_addr)
        free(cwic_socket_addr);
    if(cwic_buffer)
        free(cwic_buffer);
}