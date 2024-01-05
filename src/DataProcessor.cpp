#define MAX_BUFFER 20480

#include "DataProcessor.h"
#include "DataUtil.h"

#include <iomanip>
#include <fstream>
#include <map>




map<int, dataReference> referenceMap = {};

const string INDEX_KEY = "index";
const string DREF_KEY = "dref";
const string DESCRIPTION_KEY = "description";
const string FREQ_KEY = "freq";
const string ROUNDING_KEY = "rounding";
const string UNITS_KEY = "units";
DataUtil _util;








//========PUBLIC METHODS===========//

DataProcessor::DataProcessor()
{
    _util = DataUtil();
}

void DataProcessor::Start() {

    try{
        _util.writeLog("Connect and receive initial message");
        dataFrame df = _util.receive();
        _util.
        _util.writeLog("Begin data transfer");
        _util.send(reply);

    }
    catch(exception& ex)
    {
        char* message;
        sprintf(message, "An exception occurred: %s", ex.what());
        _util.writeLog(message);

    }

}

void DataProcessor::Stop() {

}



