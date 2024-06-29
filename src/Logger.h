//
// Created by Walter on 6/23/2024.
//

#ifndef LOGGER_H
#define LOGGER_H


#include <string>

class Logger
{
    public:
        Logger();
        void debug(std::string message);
        void info(std::string message);
        void error(std::string message);
};

#endif //LOGGER_H