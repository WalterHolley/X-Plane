//
// Created by Zero on 8/9/2024.
//

#ifndef RECORDER_H
#define RECORDER_H
#include <string>
#include "BBTypes.h"
#include "Logger.h"

class Recorder
{
public:
    Recorder(std::string &sessionId, dataFrame* df, Logger &logger);
    void write();
    bool init();
    ~Recorder();
private:
    int ec;
    Logger _log;
    bool writeSchema();
    std::string createInsertStatement(std::vector<dataStruct> values, const char* &tableName, uint64_t &timestamp, uint64_t &stateId);

};

#endif //RECORDER_H
