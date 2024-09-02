
#ifndef DATAUTIL_H
#define DATAUTIL_H

#define MAX_TIME 20
#include<XPLM/XPLMUtilities.h>
#include <string>
#include <iostream>
#include <time.h>
#include <map>
#include<vector>
#include "Logger.h"
#include "BBTypes.h"

using namespace boost;
using namespace std;




class DataUtil{
public:
    DataUtil(Logger &log);
    string dataframeToString(dataFrame &df);
    dataFrame getScenarioData(string &request);
    void updateScenario(dataFrame &df);
    ~DataUtil();






};

#endif //DATAUTIL_H
