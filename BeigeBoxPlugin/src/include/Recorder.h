//
// Created by Zero on 8/9/2024.
//

#ifndef RECORDER_H
#define RECORDER_H
#include <string>
#include "BBTypes.h"

class Recorder
{
public:
    Recorder(std::string &sessionId, dataFrame* df);
    void write();
    bool init();
    ~Recorder();
};

#endif //RECORDER_H
