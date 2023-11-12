#define MAX_BUFFER 20480

#include "DataProcessor.h"
#include "DataUtil.h"
#include "../boost/json/src.hpp"
#include <iomanip>
#include <fstream>
#include <map>


namespace  json = boost::json;

map<int, dataReference> referenceMap = {};
const json::string_view  STATES_KEY = "state";
const json::string_view INPUTS_KEY = "input";
const json::string_view INSTRUCTIONS_KEY = "instructions";
const json::string_view FAILURES_KEY = "failures";
const string INDEX_KEY = "index";
const string DREF_KEY = "dref";
const string DESCRIPTION_KEY = "description";
const string FREQ_KEY = "freq";
const string ROUNDING_KEY = "rounding";
const string UNITS_KEY = "units";
DataUtil _util;

json::stream_parser parser;




/**
 * string representation of reply element in
 * the response frame
 * @param ds
 * @return
 */
string dataStruct_to_reply_string(dataStruct ds)
{
    _util.writeLog("Writing reply string");
    char buffer[50];
    char* logMsg;

    snprintf(buffer, 50, "{\"%s\": %i, \"%s\": \"%s\" }", "index", ds.index, "value", ds.value.c_str());
    _util.writeLog(buffer);
    string jsonString = buffer;
    sprintf(logMsg, "outgoing json object: %s\n", buffer);
    _util.writeLog(buffer);
    return jsonString;
}

/**
 * populates an array for a reply message
 * @param p
 * @param dataVector
 */
void write_reply_array(json::stream_parser &p, vector<dataStruct> dataVector)
{
    for(int i = 0; i < dataVector.size(); i++)
    {
        p.write_some(dataStruct_to_reply_string(dataVector[i]));
        if(i + 1 < dataVector.size())
            p.write_some(",");
    }
}

string set_reply_message(dataFrame df)
{
    _util.writeLog("Setting reply message");
    parser.reset();
    string result;

    try
    {
        parser.write_some("{");
        parser.write_some("\"states\" : [");
        write_reply_array(parser, df.state);
        parser.write_some("],");
        parser.write_some("\"inputs\" : [");
        write_reply_array(parser, df.inputs);
        parser.write_some("],");
        parser.write_some("\"instructions\" : [");
        write_reply_array(parser, df.instructions);
        parser.write_some("],");
        parser.write_some("\"failures\" : [");
        write_reply_array(parser, df.failures);
        parser.write_some("]}");

        if(parser.done())
        {
            result = json::serialize(parser.release());
        }
        else
        {
            _util.writeLog("json reply ont completely formed");
        }
    }
    catch(exception& ex)
    {
        char* logMsg;
        sprintf(logMsg, "An exception occurred while setting the sreply message: %s\n", ex.what());
        _util.writeLog(logMsg);
    }

    return result;
}



json::value get_frame(string message)
{
    char* logMsg;
    json::error_code errorCode;

    try
    {
        parser.write_some(message, errorCode);

        if(errorCode)
        {
            sprintf(logMsg, "There was a problem during json parsing: %s", errorCode.what().c_str());
            _util.writeLog(logMsg);
        }

        parser.finish(errorCode);

        if(errorCode)
            sprintf(logMsg, "There was a wrapping up during json parsing: %s", errorCode.what().c_str());
        else
        {
            sprintf(logMsg, "message parsing completed successfully");
        }
        _util.writeLog(logMsg);
    }
    catch(exception& ex)
    {
        sprintf(logMsg, "There was a problem during json parsing: %s", ex.what());
        _util.writeLog(logMsg);
        return nullptr;
    }


    return parser.release();
}

string extract_string(json::object &jsonObj, string &paramKey)
{
    return reinterpret_cast<basic_string<char> &&>(jsonObj.at(paramKey).get_string());
}

int extract_int(json::object &jsonObj, string &paramKey)
{
    return jsonObj.at(paramKey).get_int64();
}

/**
 * maps a datastruct from a json object provided from
 * the server
 * @return
 */
dataStruct map_datastruct(json::object jsonObj)
{
    dataStruct structure;
    structure.index = jsonObj.at(INDEX_KEY).get_int64();
    structure.dref = jsonObj.at(DREF_KEY).get_string();
    structure.description = jsonObj.at(DESCRIPTION_KEY).get_string();
    structure.freq = jsonObj.at(FREQ_KEY).get_int64();
    structure.rounding = jsonObj.at(ROUNDING_KEY).get_int64();
    structure.units = jsonObj.at(UNITS_KEY).get_string();

    return structure;

}

/**
 * Gets an X-plane data reference value
 * @param index
 * @return string representation of the value
 */
string get_dataref_value(int index)
{
    string result = "";

    dataReference dr = referenceMap[index];
    result = to_string(XPLMGetDataf(dr.dataref));
    return result;
}

/**
 * Builds collection of data structures representing
 * a collection of data references and their values
 * @param arr
 * @return
 */
vector<dataStruct> get_datastructures(json::value val)
{
    vector<dataStruct> datarefs = {};
    json::array arr;
    XPLMDataRef  ref;
    if(val.is_array()){
        arr = val.get_array();

        if(!arr.empty())
        {
            auto iter = arr.begin();
            for(;;)
            {
                json::object item = iter->get_object();
                dataStruct ds = map_datastruct(item);

                if(referenceMap.find(ds.index) == referenceMap.end())
                {//not found.  add

                    ref = XPLMFindDataRef(ds.dref.c_str());
                    dataReference dr;
                    dr.dataref = ref;
                    dr.type = ds.units;
                    referenceMap[ds.index] = dr;
                }
                //get value
                ds.value = get_dataref_value(ds.index);
                datarefs.push_back(ds);

                if(++iter == arr.end())
                    break;
            }
        }

    }
    else
    {
        _util.writeLog("get_datastructures:  value passed was not an array");
    }

    return datarefs;

}

dataFrame parse_frame(json::value jsonValue)
{
    dataFrame df;
    //extract json object
    auto const& jsonObj = jsonValue.get_object();
    json::value extractedValue;
    _util.writeLog("Parsing frame");

    try
    {
        //check for states
        if(jsonObj.contains(STATES_KEY))
        {
            extractedValue = jsonObj.at(STATES_KEY);
            df.state = get_datastructures(extractedValue);
        }
        //check for inputs
        if(jsonObj.contains(INPUTS_KEY))
        {
            extractedValue = jsonObj.at(INPUTS_KEY);
            df.inputs = get_datastructures(extractedValue);
        }

        //check for instructions
        if(jsonObj.contains(INSTRUCTIONS_KEY))
        {
            extractedValue = jsonObj.at(INSTRUCTIONS_KEY);
            df.instructions = get_datastructures(extractedValue);
        }
        //check for failures
        if(jsonObj.contains(FAILURES_KEY))
        {
            extractedValue = jsonObj.at(FAILURES_KEY);
            df.failures = get_datastructures(extractedValue);
        }
    }
    catch(exception& ex)
    {
        char* logMsg;
        sprintf(logMsg, "an exception occurred in parse_frame: %s", ex.what());
        _util.writeLog(logMsg);
    }


    return df;


}

//========PUBLIC METHODS===========//

DataProcessor::DataProcessor()
{
    _util = DataUtil();
}

void DataProcessor::Start() {

    try{
        _util.writeLog("Connect and receive initial message");
        string reply = set_reply_message(parse_frame(get_frame(_util.receive())));
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



