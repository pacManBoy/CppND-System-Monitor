#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : _pid(pid) {
  _ram = std::stoi(Ram());
  _cpu = CpuUtilization();
}

int Process::Pid() const { return _pid; }

float Process::CpuUtilization() const {
  float util =  LinuxParser::CpuUtilization(_pid);
  return util;
}

string Process::Command() const {
  return LinuxParser::Command(_pid);
}

string Process::Ram() const {
  return LinuxParser::Ram(_pid);
}

string Process::User() const {
  return LinuxParser::User(_pid);
}

long int Process::UpTime() const {
  return LinuxParser::UpTime();
}

bool Process::operator<(Process const& a) const {
  return this->_cpu > a._cpu;
}

