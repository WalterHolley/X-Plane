//
// Created by Zero on 6/26/2023.
//

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "Logger.h"
#include "UDPClient.h"
#include "Recorder.h"
#include <string>
#include <vector>

class DataProcessor
{
    public:
        void init();
        bool hasInited();
        void stop();
        void get();
        DataProcessor(Logger* log); //TODO:  Figure out session Id
        ~DataProcessor();
    private:
        Logger* _log;
        Recorder* _dataRecorder;
        bool _started;
        DataUtil* _dataUtil;
};



#endif //DATAPROCESSOR_H
