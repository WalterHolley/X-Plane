//
// Created by Zero on 8/31/2024.
//

#ifndef TASKWORKER_H
#define TASKWORKER_H

#include <thread>
#include <future>
#include <queue>
#include "Logger.h"

class TaskWorker
{
public:
    void start();
    TaskWorker(Logger &logger);
    ~TaskWorker();
    bool isTaskQueued();
    bool isStarted();
    void stop();
    void push(std::function<void()> task);
private:
    std::deque<std::function<void()>> taskQueue;
    std::mutex operationMutex;
    std::future<void> workerLoopFuture;
    void executeTasks();
    Logger _log;
    bool _isStopped;
};

#endif //TASKWORKER_H