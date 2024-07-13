//
// Created by Zero on 7/11/2024.
//

#ifndef XPLANECONNECTION_H
#define XPLANECONNECTION_H

#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>

class XPlaneConnection : public boost::enable_shared_from_this<XPlaneConnection>
{
    public:
        typedef boost::shared_ptr<XPlaneConnection> pointer;
        static pointer create(boost::asio::io_service& io_service);
        boost::asio::ip::tcp::socket& getSocket();
        void start();
    private:
        boost::asio::ip::tcp::socket _socket;
        XPlaneConnection(boost::asio::io_service& io_service) : _socket(io_service);
        void handleWriteError(boost::system::error_code& error, size_t bytesXferred);
};


#endif //XPLANECONNECTION_H
