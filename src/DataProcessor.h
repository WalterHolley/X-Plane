//
// Created by Zero on 6/26/2023.
//

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../SDK/CHeaders/XPLM/XPLMDataAccess.h"
#include "DataUtil.h"
#include <string>
#include <vector>

using namespace std;


class DataProcessor
{
    public:
        void Start();
        void Stop();
        DataProcessor();

};



#endif //DATAPROCESSOR_H
