//
// Created by Walter on 6/23/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include<string>

using namespace std;

class Logger
{
    public:
        Logger();
        void debug(string message);
        void info(string message);
        void error(string message);
};

#endif LOGGER_H