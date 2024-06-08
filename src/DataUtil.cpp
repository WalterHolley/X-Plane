

#include "DataUtil.h"
#include<boost/json/src.hpp>

boost::system::error_code ec;
json::stream_parser parser;
time_t t;
map<int, dataReference> referenceMap = {};
char* outFilePath;
FILE* logFile;


void writeLog(string message)
{
    char time_string[MAX_TIME];

    strftime(time_string, MAX_TIME, "%mm-%dd-%yyyy", localtime(&t));
    fprintf(logFile, "%s: %s\n", time_string, message.c_str());
}

//***JSON PARSING**//

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

string extract_string(json::object &jsonObj, string &paramKey)
{
    return reinterpret_cast<basic_string<char> &&>(jsonObj.at(paramKey).get_string());
}

int extract_int(json::object &jsonObj, string &paramKey)
{
    return jsonObj.at(paramKey).get_int64();
}

json::value get_frame(string message)
{
    char* logMsg;
    json::error_code errorCode;
    boost::string_view bsv = boost::string_view(message);
    try
    {
        parser.write(bsv, errorCode);

        if(errorCode)
        {
            sprintf(logMsg, "There was a problem during json parsing: %s", errorCode.what().c_str());
            writeLog(logMsg);
        }

        parser.finish(errorCode);

        if(errorCode)
            sprintf(logMsg, "There was a wrapping up during json parsing: %s", errorCode.what().c_str());
        else
        {
            sprintf(logMsg, "message parsing completed successfully");
        }
        writeLog(logMsg);
    }
    catch(std::exception& ex)
    {
        sprintf(logMsg, "There was a problem during json parsing: %s", ex.what());
        writeLog(logMsg);
        return nullptr;
    }


    return parser.release();
}


/**
 * string representation of reply element in
 * the response frame
 * @param ds
 * @return
 */
string dataStruct_to_reply_string(dataStruct ds)
{
    writeLog("Writing reply string");
    char buffer[50];
    char* logMsg;

    snprintf(buffer, 50, "{\"%s\": %i, \"%s\": \"%s\" }", "index", ds.index, "value", ds.value.c_str());
    writeLog(buffer);
    string jsonString = buffer;
    sprintf(logMsg, "outgoing json object: %s\n", buffer);
    writeLog(buffer);
    return jsonString;
}

/**
 * populates an array for a reply message
 * @param p
 * @param dataVector
 */
string write_reply_array(vector<dataStruct> dataVector)
{
    string replyString = "";
    for(int i = 0; i < dataVector.size(); i++)
    {

        replyString.append(dataStruct_to_reply_string(dataVector[i]));
        if(i + 1 < dataVector.size())
            replyString.append(",");
    }

    return replyString;
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
        writeLog("get_datastructures:  value passed was not an array");
    }

    return datarefs;

}

dataFrame parse_frame(json::value jsonValue)
{
    dataFrame df;
    //extract json object
    auto const& jsonObj = jsonValue.get_object();
    json::value extractedValue;

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
    catch(std::exception& ex)
    {
        char* logMsg;
        sprintf(logMsg, "an exception occurred in parse_frame: %s", ex.what());
        writeLog(logMsg);
    }
    return df;
}

//*****PUBLIC METHODS*****//
DataUtil::DataUtil()
{
    //logging setup
    XPLMGetSystemPath(outFilePath);
    strcat(outFilePath, "TestValues.txt");
    logFile = fopen(outFilePath, "w");


}

/**
 * Sends data to server
 * @param df Frame representing the data to be sent
 * @return true if data sent successfully
 */


void DataUtil::writeToLog(string message)
{
    writeLog(message);
}

/**
 * Returns a json string of the dataframe object
 * @param df
 * @return
 */
string DataUtil::dataframeToString(dataFrame df)
{
    parser.reset();
    string result;
    char* data;


    try
    {
        sprintf(data, "{\"states\":[%s], \"inputs\":[%s], \"instructions\":[%s], \"failures\":[%s]}",
                write_reply_array(df.state).c_str(),
                write_reply_array(df.inputs).c_str(),
                write_reply_array(df.instructions).c_str(),
                write_reply_array(df.failures).c_str());

        parser.write(boost::string_view(data).substr(0, string(data).length()));

        if(parser.done())
        {
            result = json::serialize(parser.release());
        }
        else
        {
            writeLog("json message not completely formed");
        }
    }
    catch(std::exception& ex)
    {
        char* logMsg;
        sprintf(logMsg, "An exception occurred while building the json message: %s\n", ex.what());
        writeLog(logMsg);
    }

    return result;
}

dataFrame DataUtil::getScenarioData(std::string request)
{
    return parse_frame(get_frame(request));
}

DataUtil::~DataUtil()
{
    fclose(logFile);
    parser.finish();
}