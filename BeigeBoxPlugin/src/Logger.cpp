//
// Created by Walter on 6/23/2024.
//

#include "include/Logger.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <mutex>
#include <future>
#include <boost/format.hpp>
//*****PUBLIC METHODS*****//


using namespace std;
using boost::format;

ofstream logFile;
time_t t;




Logger::Logger()
{
    try
    {
        logFile.open("BB_log.txt", ios_base::app);
        info("Logging Initialized");

    }
    catch(exception& ex)
    {
        cout << "Log instancing failed: " << ex.what() << endl;
    }

}
//TODO: Work on async calls for log management

void writeToLog(const char* &level, string &message)
{
    string logEntry;
    stringstream ss;
    t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    ss << put_time(localtime(&t), "%m-%d-%Y:%X");
    logEntry = str(format("[%s]:%s=>%s") % level % ss.str() % message);
    if(logFile.is_open())
        logFile << logEntry << endl;

}

void Logger::debug(string message)
{
    writeToLog(DEBUG_LEVEL, message);

}

void Logger::info(string message)
{
    writeToLog(INFO_LEVEL, message);
    //nfo_future = async(launch::async, [msg = &message,this](){writeToLog(INFO_LEVEL, (string &)msg);});
}

void Logger::error(string message)
{
    writeToLog(ERROR_LEVEL, message);
    //error_future = async(launch::async,[ msg = &message, this](){ writeToLog( ERROR_LEVEL, (string &)msg);});

}

Logger::~Logger()
{
    //wait for all threads to finish
    info("=====LOGGING TERMINATED=====");

    //close file
    logFile.close();
}

