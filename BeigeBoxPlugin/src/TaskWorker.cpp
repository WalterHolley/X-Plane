//
// Created by Zero on 8/31/2024.
//

#include "include/TaskWorker.h"


using namespace  std;




//*****Public Methods*****//

TaskWorker::TaskWorker(Logger* logger)
{
    _log = logger;
    _isStopped = true;
}

void TaskWorker::push(std::function<void()> task)
{
    lock_guard<mutex> lockGuard(operationMutex);
    taskQueue.push_back(task);
    _log->debug("TaskWorker: Task Queued");
}

bool TaskWorker::isTaskQueued()
{
    lock_guard<mutex> lockGuard(operationMutex);
    return !taskQueue.empty();
}

void TaskWorker::start()
{
    if(_isStopped || !workerLoopFuture.valid())
    {
       workerLoopFuture = async(&TaskWorker::executeTasks, this);
       lock_guard<mutex> lockGuard(operationMutex);
       _isStopped = false;
       _log->info("TaskWorker: Task Worker Started");
    }

}

void TaskWorker::stop()
{
    lock_guard<mutex> lockGuard(operationMutex);
    _isStopped = true;
    workerLoopFuture.wait();
    _log->info("Task Worker Stopped");
}

bool TaskWorker::isStarted()
{
    return !_isStopped;
}

void TaskWorker::executeTasks()
{
    while(!_isStopped)
    {
        if(!taskQueue.empty())
        {
            auto func = taskQueue.front();
            packaged_task<void()> task(func);

            thread t(std::move(task));

            _log->debug("Task started");
            lock_guard<mutex> lockGuard(operationMutex);
            t.join();
            taskQueue.pop_front();
            _log->debug("Task completed");
        }

    }

}