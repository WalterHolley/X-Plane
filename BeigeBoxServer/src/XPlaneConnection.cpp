//
// Created by Zero on 7/11/2024.
//

#include "include/XPlaneConnection.h"

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


