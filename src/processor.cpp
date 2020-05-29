#include "processor.h"

#include <string>

// DPNE: Return the aggregate CPU utilization
float Processor::Utilization() {
  CPU_utilization = std::stof(LinuxParser::CpuUtilization());
  return CPU_utilization;
}