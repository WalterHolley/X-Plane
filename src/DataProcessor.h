//
// Created by Zero on 6/26/2023.
//

#ifndef X_PLANE_DATAPROCESSOR_H
#define X_PLANE_DATAPROCESSOR_H

#include <string>

using namespace std;



struct dataStruct {
    string dref;
    string description;
    int index;
    string units;
    int freq;
    int rounding;

};

struct dataFrame{
    dataStruct* state;
    dataStruct* inputs;
    dataStruct* instructions;
    dataStruct* failures;
};

struct dataReference {
    string dataref;
    string value;
};


class DataProcessor
{
    public:
        void Start();
        void Stop();


};



#endif //X_PLANE_DATAPROCESSOR_H
