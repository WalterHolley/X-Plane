//
// Created by Zero on 6/26/2023.
//

#define MAX_BUFFER 20480

#include "DataProcessor.hpp"
#include "../SDK/CHeaders/XPLM/XPLMUtilities.h"
#include "../boost/json/src.hpp"
#include <iomanip>
#include <fstream>
#include <map>

namespace  json = boost::json;
map<int, dataReference> referenceMap = {};
json::string_view  STATES_KEY = "state";
json::string_view INPUTS_KEY = "input";
json::string_view INSTRUCTIONS_KEY = "instructions";
json::string_view FAILURES_KEY = "failures";
json::stream_parser parser;
char const* fileName = "F:\\X-Plane 12\\Resources\\plugins\\BeigeBox\\win_x64\\datarefs.json";

FILE * logFile;
char outFilePath[512];

/**
 * string representation of reply element in
 * the response frame
 * @param ds
 * @return
 */
string dataStruct_to_reply_string(dataStruct ds)
{
    fprintf(logFile, "writing reply string\n");
    char buffer[50];

    snprintf(buffer, 50, "{\"%s\": %i, \"%s\": \"%s\" }", "index", ds.index, "value", ds.value.c_str());
    string jsonString = buffer;
    fprintf(logFile, "outgoing json object: %s\n", buffer);
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
    fprintf(logFile, "setting reply message\n");
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
            fprintf(logFile, "json reply not completely formed\n");
        }
    }
    catch(exception& ex)
    {
        fprintf(logFile, "An exception occurred while setting the sreply message: %s\n", ex.what());
    }



    return result;
}


//TODO: Gets from file for now, will read from UDP stream later?
json::value get_frame()
{
    string line;

    json::error_code errorCode;

    try
    {
        ifstream testFile(fileName);
        if(testFile.is_open())
        {
            while(getline(testFile, line) && (!errorCode))
            {

                parser.write_some(line, errorCode);
            }
            testFile.close();


            if(errorCode)
                fprintf(logFile, "There was a problem during json parsing: %s", errorCode.what().c_str());

            parser.finish(errorCode);

            if(errorCode)
                fprintf(logFile, "There was a wrapping up during json parsing: %s", errorCode.what().c_str());
        }
        else
        {
            fprintf(logFile, "json file not opened\n");
        }

    }
    catch(exception& ex)
    {
        fprintf(logFile, "There was a problem during json parsing: %s", ex.what());
        return nullptr;
    }


    return parser.release();
}

/**
 * maps a datastruct from a json object provided from
 * the server
 * @return
 */
dataStruct map_datastruct(json::object jsonObj)
{
    dataStruct structure;
    structure.index = jsonObj.at("index").get_int64();
    structure.dref = jsonObj.at("dref").get_string();
    structure.description = jsonObj.at("description").get_string();
    structure.freq = jsonObj.at("freq").get_int64();
    structure.rounding = jsonObj.at("rounding").get_int64();
    structure.units = jsonObj.at("units").get_string();

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
                    fprintf(logFile, "New data ref found: %s\n",ds.dref.c_str());
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
        fprintf(logFile, "get_datastructures:  value passed was not an array\n");
    }

    return datarefs;

}

dataFrame parse_frame(json::value jsonValue)
{
    dataFrame df;
    //extract json object
    auto const& jsonObj = jsonValue.get_object();
    json::value extractedValue;
    fprintf(logFile, "Parsing frame\n");

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
        fprintf(logFile, "an exception occurred in parse_frame: %s", ex.what());
        return df;
    }


    return df;


}

//========PUBLIC METHODS===========//


void DataProcessor::Start() {

    //create path for log
    //TODO: Consider keeping log, but moving to resource folder
    XPLMGetSystemPath(outFilePath);
    strcat(outFilePath, "TestValues.txt");

    try{
        logFile = fopen(outFilePath, "w");

        string reply = set_reply_message(parse_frame(get_frame()));
        fprintf(logFile, "%s\n", reply.c_str());
        fclose(logFile);

    }
    catch(exception& ex)
    {
        fprintf(logFile, "An exception occurred: %s", ex.what());
        fclose(logFile);
    }

}

void DataProcessor::Stop() {
    fclose(logFile);
}



