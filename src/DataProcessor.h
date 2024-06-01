//
// Created by Zero on 6/26/2023.
//

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H


#include "UDPClient.h"
#include <string>
#include <vector>

using namespace std;


class DataProcessor
{
    public:
        void Start();
        void Stop();
        void get();
        DataProcessor();
        ~DataProcessor();
    private:
        UDPClient* _client;
        DataUtil* _util;
};



#endif //DATAPROCESSOR_H
