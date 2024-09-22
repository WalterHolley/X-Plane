#define MAX_BUFFER 20480


#include "include/DataProcessor.h"
#include <thread>
#include <fstream>
#include <filesystem>
#include<sstream>
#include <string>


using namespace std;

dataFrame df;
string TEMP_SESSION = "NORTHWIND_AI";
future<void> processingFuture;
int frameRate = 1000 / 10;



//========CLASS METHODS===========//

DataProcessor::DataProcessor() {}

DataProcessor::DataProcessor(Logger *log)
{
    _started = false;
    _inited = false;
    _dataUtil = new DataUtil(log);
    _taskWorker = new TaskWorker(log);
    _log = log;
    _log->info("DataProcessor: Instanced");
}

void DataProcessor::init()
{
    if(!_inited)
    {
        try
        {
            //Load json
            stringstream content;
            string jsn;
            _log->debug("DataProcessor: Current path: " + filesystem::current_path().string());
            ifstream json("datarefs.json");
            if(json.good())
            {
                while(getline(json, jsn))
                {
                    content << jsn;
                }

                //convert to dataframe
                jsn = content.str();
                df = _dataUtil->getScenarioData(jsn);
                _dataRecorder = new Recorder(TEMP_SESSION, _log);
                _inited = _dataRecorder->init(df);
                json.close();
                if(_inited)
                    _log->info("DataProcessor: Initialization successful");
            }
            else
            {
                _log->error("DataProcessor: Init failed. Could not open json file");
            }



        }
        catch(std::exception& ex)
        {
            string message("DataProcessor: Init Failed: ");
            message.append(ex.what());
            _log->error(message);
            _started = false;
        }
    }


}

shared_future<void> DataProcessor::start()
{
    if(_inited && !_started)
    {
        packaged_task<void()> pt([this] {dataLoop();});;
        processingFuture = pt.get_future();
        std::thread launchThread(std::move(pt));

        _started = true;
    }

    return processingFuture.share();
}


void DataProcessor::stop()
{
    //disconnect from db
    _started = false;

}

bool  DataProcessor::hasInited()
{
    return _inited;
}


void DataProcessor::dataLoop()
{
    while (_started) {
        std::chrono::time_point start = std::chrono::steady_clock::now() + std::chrono::milliseconds(frameRate);
        _dataUtil->updateScenario(df);
        if(!_taskWorker->isStarted())
        {
            _taskWorker->start();
            _log->info("DataProcessor: Task Worker Started");
        }
        auto task = [this](){_dataRecorder->write(df);};
        _taskWorker->push(task);
        std::this_thread::sleep_until(start);
    }
    _taskWorker->stop();
}

void DataProcessor::writeLoop()
{
    _dataUtil->updateScenario(df);
    _dataRecorder->write(df);
}


DataProcessor::~DataProcessor()
{
    free(_dataRecorder);
    free(_dataUtil);
    free(_taskWorker);
}


