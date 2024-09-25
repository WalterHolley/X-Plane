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
    _log->info("TaskWorker: Task Worker Stopped");
}

bool TaskWorker::isStarted()
{
    return !_isStopped;
}

void TaskWorker::executeTasks()
{
    while(!_isStopped)
    {
        unique_lock<std::mutex> queueLock(operationMutex);
        queueCondition.wait(queueLock, [this](){
            _log->debug("Checking for tasks");
            return !taskQueue.empty();
        });

        auto func = taskQueue.front();
        taskQueue.pop_front();
        packaged_task<void()> task(func);

        _log->debug("TaskWorker: Executing Task");
        thread t(std::move(task));
        lock_guard<mutex> lockGuard(operationMutex);
        t.join();
        _log->debug("TaskWorker: Executed Task completed");


    }

}