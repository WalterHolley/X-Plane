//
// Created by Zero on 6/26/2023.
//

#include "DataProcessor.hpp"
#include <boost/json/src.hpp>
#include <iomanip>
#include <fstream>
#include <map>

namespace  json = boost::json;
map<int, dataReference> referenceMap = {};
json::string_view  STATES_KEY = "state";
json::string_view INPUTS_KEY = "input";
json::string_view INSTRUCTIONS_KEY = "instructions";
json::string_view FAILURES_KEY = "failures";
char const* fileName = "../test/datarefs.json";





//TODO: Gets from file for now, will read from UDP stream later?
json::value get_frame()
{
    char* buffer;
    string line;
    ifstream testFile(fileName);
    json::error_code errorCode;
    json::stream_parser parser;


    while(getline(testFile, line))
    {
        strcat(buffer, line.c_str());
    }
    testFile.close();

    parser.write(buffer, sizeof (buffer), errorCode);

    if(errorCode)
        return nullptr;

    parser.finish(errorCode);

    if(errorCode)
        return nullptr;

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
    string result = NULL;

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

    return datarefs;

}

dataFrame parse_frame(json::value jsonValue)
{
    dataFrame df;
    //extract json object
    auto const& jsonObj = jsonValue.get_object();
    json::value extractedValue;

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

    return df;


}

//========PUBLIC METHODS===========//


void DataProcessor::Start() {
    parse_frame(get_frame());
}

void DataProcessor::Stop() {

}



