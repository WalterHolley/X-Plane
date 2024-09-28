//
// Created by Zero on 7/11/2024.
//

#ifndef XPLANECONNECTION_H
#define XPLANECONNECTION_H

#include<deque>
#include<string>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/asio/awaitable.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>
#include<boost/asio/use_awaitable.hpp>


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
        void handleMessage(boost::asio::streambuf& buff);
        void handleWriteError(boost::system::error_code& error, size_t bytesXferred);
        std::deque<std::string> _msgWriter;
};


#endif //XPLANECONNECTION_H
