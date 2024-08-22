#define MAX_BUFFER 20480

#include "include/DataProcessor.h"
#include "include/DataUtil.h"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <fstream>
#include <string>

using namespace std;
using namespace boost;

dataFrame df;
dataFrame* dfPtr;
string TEMP_SESSION = "NORTHWIND_AI";
boost::asio::thread_pool taskPool(1);
boost::thread writeThread();
int frameRate = 1000 / 60;

void writeLoop()
{

}


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
}


void DataProcessor::stop()
{
    //disconnect from db

}

bool  DataProcessor::hasInited()
{
    return _started;
}


void DataProcessor::dataLoop()
{

}

DataProcessor::~DataProcessor()
{
    free(_dataRecorder);
    free(_dataUtil);
    free(dfPtr);
}


