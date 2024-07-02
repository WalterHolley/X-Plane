//
// Created by Walter on 6/23/2024.
//

#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <mutex>
#include <future>
//*****PUBLIC METHODS*****//


using namespace std;

ofstream logFile;
time_t t;
mutex log_mutex;
future<void> info_future;
future<void> debug_future;
future<void> error_future;

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

void writeToLog(const char* &level, string &message)
{
    const lock_guard<mutex> lock(log_mutex);
    char* logEntry;
    stringstream  ss;
    auto current_time = chrono::system_clock::now();
    t = chrono::system_clock::to_time_t(current_time);
    ss << put_time(localtime(&t), "%m-%d-%Y:%X");
    sprintf(logEntry, "[%s]:%s=>%s", level, ss.str().c_str(), message.c_str());
    if(logFile.is_open())
        logFile << logEntry << endl;

}

void Logger::debug(string message)
{
    debug_future = async(launch::async, [msg = &message,this](){writeToLog(DEBUG_LEVEL, (string &)msg);});
}

void Logger::info(string message)
{
    info_future = async(launch::async, [msg = &message,this](){writeToLog(INFO_LEVEL, (string &)msg);});
}

void Logger::error(string message)
{
    error_future = async(launch::async,[ msg = &message, this](){ writeToLog( ERROR_LEVEL, (string &)msg);});

}

Logger::~Logger()
{
    //wait for all threads to finish
    info("=====LOGGING TERMINATED=====");
    debug_future.wait();
    info_future.wait();
    error_future.wait();

    //close file
    logFile.close();
}

