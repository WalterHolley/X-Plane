#define MAX_BUFFER 20480

#include "include/DataProcessor.h"
#include "include/DataUtil.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <fstream>
#include <string>
#include <ctime>

using namespace std;
using namespace boost;



dataFrame df;
dataFrame* dfPtr;
string TEMP_SESSION = "NORTHWIND_AI";
boost::asio::thread_pool taskPool(1);
int frameRate = 1000 / 60;






//========CLASS METHODS===========//

DataProcessor::DataProcessor(Logger* log)
{
    _started = false;
    _dataUtil = new DataUtil(log);
    _log = log;
}

void DataProcessor::init()
{
    if(!_started)
    {
        try
        {
            //Load json
            char* content;
            ifstream json;
            json.open("../res/datarefs.json");
            json.read(content, json.gcount());
            //convert to dataframe
            string jsn(content);
            df = _dataUtil->getScenarioData(jsn);
            dfPtr = new dataFrame;
            *dfPtr = df;
            _dataRecorder = new Recorder(TEMP_SESSION, dfPtr, _log);

        }
        catch(std::exception& ex)
        {
            string message("An exception occurred while initializing data processing: ");
            message.append(ex.what());
            _log->error(message);
            _started = false;
        }
    }


}

void DataProcessor::start()
{
    if(!_started)
    {
        boost::thread(dataLoop);
    }
}


void DataProcessor::stop()
{
    //disconnect from db
    _started = false;

}

bool  DataProcessor::hasInited()
{
    return _started;
}


void DataProcessor::dataLoop()
{
    while (_started) {
        boost::chrono::time_point start = boost::chrono::steady_clock::now() + boost::chrono::milliseconds(frameRate);
        auto task = bind(&DataProcessor::writeLoop, this);
        boost::asio::post(taskPool, task);
        boost::this_thread::sleep_until(start);
    }
    taskPool.join();
}

void DataProcessor::writeLoop()
{
    _dataUtil->updateScenario(df);
    _dataRecorder->write();
}


DataProcessor::~DataProcessor()
{
    free(_dataRecorder);
    free(_dataUtil);
    free(dfPtr);
}


