//
// Created by Zero on 7/11/2024.
//

#include "include/XPlaneListener.h"

using boost::asio::ip::tcp;

//*****PUBLIC METHODS*****//

XPlaneListener::XPlaneListener(boost::asio::io_service &ioService) :
        _acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13))
{

}

void XPlaneListener::start()
{
    XPlaneConnection::pointer newConnection = XPlaneConnection::create(_acceptor.)
}

void XPlaneListener::acceptConnection(XPlaneConnection::pointer newConnection, const boost::system::error_code& errorCode)
{
    if(!errorCode)
    {
        newConnection->start();
        start();
    }
}