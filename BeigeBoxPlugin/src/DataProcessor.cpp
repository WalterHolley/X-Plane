#define MAX_BUFFER 20480

#include "include/DataProcessor.h"
#include "include/DataUtil.h"

#include <fstream>
#include <string>

dataFrame df;

using namespace std;

//========PUBLIC METHODS===========//

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
            _dataRecorder = new Recorder(df, _log);


        }
        catch(std::exception& ex)
        {
            string message("An exception occurred during data processing: ");
            message.append(ex.what());
            _log->error(message);
            _started = false;
        }
    }


}

void DataProcessor::get()
{

}


void DataProcessor::stop()
{
    //disconnect from db

}

bool  DataProcessor::hasInited() {
    return _started;
}


DataProcessor::~DataProcessor()
{
    free(_dataRecorder);
    free(_dataUtil);
}

