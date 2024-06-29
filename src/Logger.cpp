//
// Created by Walter on 6/23/2024.
//

#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
//*****PUBLIC METHODS*****//


using namespace std;

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

void writeToLog(string level, string &message)
{
    char logEntry[512];
    stringstream  ss;
    auto current_time = chrono::system_clock::now();
    t = chrono::system_clock::to_time_t(current_time);
    ss << put_time(localtime(&t), "%m-%d-%Y:%X");
    sprintf(logEntry, "[%s]:%s=>%s", level.c_str(), ss.str().c_str(), message.c_str());
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

}

void Logger::error(string message)
{
    writeToLog(ERROR_LEVEL, message);
}

Logger::~Logger()
{
    logFile.close();
}