//
// Created by Zero on 7/11/2024.
//

#ifndef XPLANELISTENER_H
#define XPLANELISTENER_H

#include "include/XPlaneConnection.h"
#include <boost/asio.hpp>

class XPlaneListener
{
    public:
        XPlaneListener(boost::asio::io_service& ioService);
    private:
        void acceptConnection(XPlaneConnection::pointer newConnection, const boost::system::error_code& errorCode);
        void start();
        void stop();
        boost::asio::ip::tcp::acceptor _acceptor;
};


#endif //XPLANELISTENER_H
