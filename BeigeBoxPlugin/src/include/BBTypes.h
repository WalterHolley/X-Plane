// Copyright (c) 2024 Walter Holley III. All Rights Reserved.

#ifndef BBTYPES_H
#define BBTYPES_H

#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMUtilities.h>
#include <boost/utility/string_view.hpp>
#include <map>
#include <string>
#include <vector>

const boost::string_view STATES_KEY = "state";
const boost::string_view INPUTS_KEY = "input";
const boost::string_view INSTRUCTIONS_KEY = "instructions";
const boost::string_view FAILURES_KEY = "failures";
const std::string INDEX_KEY = "index";
const std::string DREF_KEY = "dref";
const std::string DESCRIPTION_KEY = "description";
const std::string FREQ_KEY = "freq";
const std::string ROUNDING_KEY = "rounding";
const std::string UNITS_KEY = "units";
const static char *unitStrings[] = {
    "deg/min", "deg", "feet", "knots",        "bool", "inHg",
    "m/s",     "m",   "s",    "float",        "int",  "ratio",
    "",        "hz",  "MHz",  "kts indicated"};
const static char *dataElementStrings[] = {"STATE", "INPUTS", "INSTRUCTIONS",
                                           "FAILURES"};

enum dataElement {
  STATE = 0,
  INPUTS = 1,
  INSTRUCTIONS = 2,
  FAILURES = 3

};
enum units {
  DEGMIN = 0,
  DEG = 1,
  FEET = 2,
  KTS = 3,
  TRUEFALSE = 4,
  INHG = 5,
  METERSPERSECOND = 6,
  METERS = 7,
  SECONDS = 8,
  FLOATINGPOINT = 9,
  INTEGER = 10,
  RATIO = 11,
  TEXT = 12,
  HZ = 13,
  MHZ = 14,
  KTIAS = 15,

};

struct dataStruct {
  std::string dref;
  std::string description;
  int index;
  std::string units;
  int freq;
  int rounding;
  std::string value;
  enum units unitsEnum;
};

struct dataFrame {
  std::vector<dataStruct> state;
  std::vector<dataStruct> inputs;
  std::vector<dataStruct> instructions;
  std::vector<dataStruct> failures;
};

struct dataReference {
  XPLMDataRef dataref;
  std::string type;
};

static std::map<const char *, units> unitMappings = {
    {unitStrings[0], units::DEGMIN},
    {unitStrings[1], units::DEG},
    {unitStrings[2], units::FEET},
    {unitStrings[3], units::KTS},
    {unitStrings[4], units::TRUEFALSE},
    {unitStrings[5], units::INHG},
    {unitStrings[6], units::METERSPERSECOND},
    {unitStrings[7], units::METERS},
    {unitStrings[8], units::SECONDS},
    {unitStrings[9], units::FLOATINGPOINT},
    {unitStrings[10], units::INTEGER},
    {unitStrings[11], units::RATIO},
    {unitStrings[12], units::TEXT},
    {unitStrings[13], units::HZ},
    {unitStrings[14], units::MHZ},
    {unitStrings[15], units::KTIAS}};

static std::map<dataElement, const char *> dataElementString = {
    {dataElement::STATE, dataElementStrings[0]},
    {dataElement::INPUTS, dataElementStrings[1]},
    {dataElement::INSTRUCTIONS, dataElementStrings[2]},
    {dataElement::FAILURES, dataElementStrings[3]}};

static std::map<const char *, dataElement> stringToDataElement = {
    {dataElementStrings[0], dataElement::STATE},
    {dataElementStrings[1], dataElement::INPUTS},
    {dataElementStrings[2], dataElement::INSTRUCTIONS},
    {dataElementStrings[3], dataElement::FAILURES}};

#endif // BBTYPES_H
