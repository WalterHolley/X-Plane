//
// Created by Zero on 7/11/2024.
//

#include "include/XPlaneConnection.h"
#include <iostream>
#include <fstream>
#include <String>

using boost::asio::ip::tcp;
using namespace std;

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
    boost::asio::streambuf readBuffer;
    boost::asio::read(_socket, readBuffer);



}

void XPlaneConnection::handleMessage(boost::asio::streambuf& buff)
{
     istream istream(&buff);
     ifstream refFile("../res/datarefs.json");
     string msg;
     istream >> msg;
     cout << msg << std::endl;

     //send dataref schema
     if(refFile.is_open())
     {
         while(getline(refFile, msg) && _socket.is_open())
         {
             boost::asio::async_write(_socket, msg,);
         }
     }


}
//********END CLASS METHODS***********//




