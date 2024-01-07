//
// Created by zero on 1/5/24.
//

#include "UDPClient.h"


boost::asio::io_service io_service;
udp::socket _socket(io_service);


//*****PUBLIC METHODS*****//
UDPClient::UDPClient()
{
    //CWIC resource init
    cwic = cwic_init();
    cwic_socket_addr = (int8_t*)malloc(sizeof(int8_t));
    cwic_buffer = (char*)malloc(MAX_CWIC_BUFFER*sizeof(char));

    //UDP connection setup
    cwic_cwic_sock_addr(cwic, cwic_socket_addr);
    memcpy(cwic_buffer, &cwic_socket_addr, sizeof(cwic_socket_addr));
    _endpoint = udp::endpoint(address::from_string(cwic_buffer), REMOTE_PORT);
    _socket.open(udp::v4());
}

bool UDPClient::send(dataFrame df)
{
    bool result = false;
    try
    {
        string message = _util.dataframeToString(df);
        _socket.send_to(boost::asio::buffer(message), _endpoint);
        result = true;
    }
    catch(exception &ex)
    {
        string msg = "There was a problem sending the data to cwic: ";
        msg.append(ex.what());

        _util.writeToLog(msg);
        if(_socket != nullptr_t)
        {
            _socket.close();
        }
    }
    return result;
}

dataFrame UDPClient::receive()
{

    dataFrame returnValue;

    try
    {
        buffer = nullptr;
        _socket.receive(boost::asio::buffer(buffer));

        if(ec)//handle socket error
        {
            _socket.close();
            returnValue = nullptr_t;
        }
        else
        {
            string result(buffer);
            returnValue = parse_frame(get_frame(result));
        }
    }
    catch(exception &ex)
    {
        _socket.close();
    }

    return returnValue;
}

UDPClient::~UDPClient()
{
    _socket.close();
    free(cwic);
    buffer = nullptr;
    if(cwic_socket_addr)
        free(cwic_socket_addr);
    if(cwic_buffer)
        free(cwic_buffer);
}