//
// Created by zero on 1/5/24.
//


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
UDPClient::UDPClient(Logger* log)
{   _log = log;
    //CWIC resource init
    _log->debug("initializing cwic");
    cwic = cwic_init();
    cwic_socket_addr = (int8_t*)malloc(sizeof(int8_t));
    cwic_buffer = (char*)malloc(MAX_CWIC_BUFFER*sizeof(char));
    _util = new DataUtil(log);
    //UDP connection setup
    cwic_cwic_sock_addr(cwic, cwic_socket_addr);
    memcpy(cwic_buffer, &cwic_socket_addr, sizeof(cwic_socket_addr));

}

void UDPClient::open()
{
    boost::system::error_code ec;
    _endpoint = udp::endpoint(address::from_string(cwic_buffer), REMOTE_PORT);
    _socket.open(udp::v4(), ec);

    if(ec)
    {
        _log->error("There was a problem opening a socket to cwic: " + ec.message());
    }
}

void UDPClient::close()
{
    if(_socket.is_open())
    {
        _socket.close();
        _log->info("cwic connection closed");
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
        _log->debug("Data sent to cwic");
    }
    catch(std::exception &ex)
    {
        string msg = "There was a problem sending the data to cwic: ";
        msg.append(ex.what());

        _log->error(msg);

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
            _log->debug("Data received from cwic");
            returnValue = _util->getScenarioData(buffer.c_array());
        }
        else{
            _log->error("Data connection is not available.");
        }
    }
    catch(std::exception &ex)
    {
        char* errorMessage;
        strcat(errorMessage, "An exception has occurred while receiving a message, and the connection has been close. ");
        strcat(errorMessage, ex.what());
        _socket.close();
        _log->error( errorMessage);

    }

    return returnValue;
}

UDPClient::~UDPClient()
{
    _socket.close();
    free(cwic);
    if(cwic_socket_addr)
        free(cwic_socket_addr);
    if(cwic_buffer)
        free(cwic_buffer);
}