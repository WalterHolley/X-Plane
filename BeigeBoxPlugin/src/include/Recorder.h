//
// Created by Zero on 8/9/2024.
//

#ifndef RECORDER_H
#define RECORDER_H
#include "BBTypes.h"
#include "Logger.h"
#include <cstdint>
#include <string>

class Recorder {
public:
  Recorder(std::string &sessionId, Logger *logger);
  void write(dataFrame const &df);
  bool init(dataFrame &df);
  ~Recorder();

private:
  int ec;
  Logger *_log;
  bool writeSchema(dataFrame &df);
  std::string createInsertStatement(std::vector<dataStruct> values,
                                    const char *&tableName,
                                    std::uint64_t &timestamp,
                                    std::uint64_t stateId);
};

#endif // RECORDER_H
