//
// Created by Zero on 6/26/2023.
//

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H


#include "DataUtil.h"
#include "UDPClient.h"
#include <string>
#include <vector>

using namespace std;


class DataProcessor
{
    public:
        void Start();
        void Stop();
        DataProcessor();
        ~DataProcessor();
    private:
        DataUtil _util;
        UDPClient _client;
};



#endif //DATAPROCESSOR_H
