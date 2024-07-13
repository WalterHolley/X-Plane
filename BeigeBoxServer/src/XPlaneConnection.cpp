//
// Created by Zero on 7/11/2024.
//

#include "include/XPlaneConnection.h"
#include <iostream>

using boost::asio::ip::tcp;

//*****CLASS METHODS*****//

/**
 * Creates a new connection element for managing an individual connection
 * @param io_service
 * @return
 */
 XPlaneConnection::pointer XPlaneConnection::create(boost::asio::io_service &io_service)
{
    return pointer(new XPlaneConnection(io_service));
}

tcp::socket& XPlaneConnection::getSocket()
{
    return _socket;
}

void XPlaneConnection::handleWriteError(boost::system::error_code& error, size_t bytesMoved)
{
    std::cout << error.what();
}

void XPlaneConnection::start()
{
     boost::asio::async_write(_socket, boost::asio::buffer(),
                              boost::bind(&XPlaneConnection::handleWriteError, shared_from_this(),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred))
}


