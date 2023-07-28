//
// Created by Zero on 6/26/2023.
//

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include <string>
#include <vector>

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


class DataProcessor
{
    public:
        void Start();
        void Stop();


};



#endif //DATAPROCESSOR_H
