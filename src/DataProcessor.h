//
// Created by Zero on 6/26/2023.
//

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "Logger.h"
#include "UDPClient.h"
#include <string>
#include <vector>

class DataProcessor
{
    public:
        void Start();
        bool hasStarted();
        void Stop();
        void get();
        DataProcessor(Logger* log);
        ~DataProcessor();
    private:
        UDPClient* _client;
        Logger* _log;
        bool _started;
};



#endif //DATAPROCESSOR_H
