#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "linux_parser.h"
#include <string>

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
  float CPU_utilization;
};

#endif