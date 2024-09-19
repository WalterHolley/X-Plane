//
// Created by Zero on 6/26/2023.
//

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "Logger.h"
#include "DataUtil.h"
#include "Recorder.h"
#include "TaskWorker.h"
#include <string>
#include <vector>
#include <future>

class DataProcessor
{
    public:
        void init();
        bool hasInited();
        void stop();
        std::shared_future<void> start();
        DataProcessor();
        DataProcessor(Logger *log); //TODO:  Figure out session Id
        ~DataProcessor();
    private:
        Logger* _log;
        Recorder* _dataRecorder;
        TaskWorker* _taskWorker;
        bool _started;
        bool _inited;
        DataUtil* _dataUtil;
        void dataLoop();
        void writeLoop();
};



#endif //DATAPROCESSOR_H
