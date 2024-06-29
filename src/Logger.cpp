//
// Created by Walter on 6/23/2024.
//

#include "Logger.h"
#include <iostream>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
//*****PUBLIC METHODS*****//

namespace logging = boost::log;
using namespace std;

Logger::Logger()
{
    try
    {
        ifstream settingFile("settings.txt");
        logging::init_from_stream(settingFile);
    }
    catch(exception& ex)
    {
        cout << "Long instancing failed: " << ex.what() << endl;
    }

}

void Logger::debug(string message)
{
    BOOST_LOG_TRIVIAL(debug) << message;
}

void Logger::info(string message)
{
    BOOST_LOG_TRIVIAL(info) << message;

}

void Logger::error(string message)
{
    BOOST_LOG_TRIVIAL(error) << message;
}

