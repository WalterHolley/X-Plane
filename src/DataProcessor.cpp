//
// Created by Zero on 6/26/2023.
//

#include "DataProcessor.h"
#include "../SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include <boost/json.hpp>
#include <iomanip>
#include <fstream>
#include <map>

namespace  json = boost::json;
map<int, XPLMDataRef> referenceMap = {};
char const* STATES_KEY = "state";
char const* INPUTS_KEY = "input";
char const* INSTRUCTIONS_KEY = "instructions";
char const* FAILURES_KEY = "failures";
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

void parse_frame(json::value jsonValue)
{
    //extract json object

    //check for states

    //check for inputs

    //check for instructions

    //check for failures





}

//========PUBLIC METHODS===========//


void DataProcessor::Start() {
    parse_frame(get_frame());
}

void DataProcessor::Stop() {

}

