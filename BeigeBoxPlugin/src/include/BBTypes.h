//
// Created by Zero on 8/9/2024.
//

#ifndef BBTYPES_H
#define BBTYPES_H

#include <XPLM/XPLMUtilities.h>
#include <XPLM/XPLMDataAccess.h>
#include <boost/utility/string_view.hpp>
#include <vector>
#include <string>


struct dataStruct {
    std::string dref;
    std::string description;
    int index;
    std::string units;
    int freq;
    int rounding;
    std::string value;

};

struct dataFrame{
    std::vector<dataStruct> state;
    std::vector<dataStruct> inputs;
    std::vector<dataStruct> instructions;
    std::vector<dataStruct> failures;
};

struct dataReference {
    XPLMDataRef dataref;
    std::string type;
};

const boost::string_view  STATES_KEY = "state";
const boost::string_view INPUTS_KEY = "input";
const boost::string_view INSTRUCTIONS_KEY = "instructions";
const boost::string_view FAILURES_KEY = "failures";
const std::string INDEX_KEY = "index";
const std::string DREF_KEY = "dref";
const std::string DESCRIPTION_KEY = "description";
const std::string FREQ_KEY = "freq";
const std::string ROUNDING_KEY = "rounding";
const std::string UNITS_KEY = "units";

#endif //BBTYPES_H
