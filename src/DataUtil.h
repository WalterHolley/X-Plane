
#ifndef DATAUTIL_H
#define DATAUTIL_H

#define MAX_TIME 20

#include<XPLM/XPLMUtilities.h>
#include<XPLM/XPLMDataAccess.h>
#include<boost/json.hpp>
#include <string>
#include <cstring>
#include <iostream>
#include <time.h>
#include <map>
#include<vector>






namespace  json = boost::json;
using namespace std;



struct dataStruct {
    string dref;
    string description;
    int index;
    string units;
    int freq;
    int rounding;
    string value;

};

struct dataFrame{
    vector<dataStruct> state;
    vector<dataStruct> inputs;
    vector<dataStruct> instructions;
    vector<dataStruct> failures;
};

struct dataReference {
    XPLMDataRef dataref;
    string type;
};

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



class DataUtil{
public:
    DataUtil();
    string dataframeToString(dataFrame df);
    dataFrame getScenarioData(string request);
    void writeToLog(string message);
    ~DataUtil();




};

#endif //DATAUTIL_H
