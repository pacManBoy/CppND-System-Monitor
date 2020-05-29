#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;

  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float MemTotal = 0, MemFree = 0, Buffers = 0, Cached = 0;
  std::ifstream FileReader(kProcDirectory + kMeminfoFilename);
  if (FileReader.is_open()) {
    while (std::getline(FileReader, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = std::stof(value);
        } else if (key == "MemFree:") {
          MemFree = std::stof(value);
        } else if (key == "Buffers:") {
          Buffers = std::stof(value);
        } else if (key == "Cached:") {
          Cached = std::stof(value);
        } else continue;
      }
    }
  }

  return (MemTotal-MemFree-Buffers-Cached) / MemTotal;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, key;
  std::ifstream FileReader(kProcDirectory + kUptimeFilename);
  if (FileReader.is_open()) {
    std::getline(FileReader, line);
    std::istringstream StringReader(line);
    StringReader >> key;
  }
  return std::stol(key);
}


// Done: Read and return CPU utilization
/*
void LinuxParser::IndividualCpuCalculation(
    std::string &line,
    std::string &result)
{

  std::istringstream reader(line);
  vector<long> CpuData(10);
  int count = 0;
  std::string token;

  while(reader >> token) {
    if (count == 0) {} // cpu_name
    if (count == 1) CpuData[kUser_] = std::stoi(token);
    if (count == 2) CpuData[kNice_] = std::stoi(token);
    if (count == 3) CpuData[kSystem_] = std::stoi(token);
    if (count == 4) CpuData[kIdle_] = std::stoi(token);
    if (count == 5) CpuData[kIOwait_] = std::stoi(token);
    if (count == 6) CpuData[kIRQ_] = std::stoi(token);
    if (count == 7) CpuData[kSoftIRQ_] = std::stoi(token);
    if (count == 8) CpuData[kSteal_] = std::stoi(token);
    if (count == 9) CpuData[kGuest_] = std::stoi(token);
    if (count == 10) CpuData[kGuestNice_] = std::stoi(token);
    count++;
  }

  static long PrevTotal = 0, PrevIdle = 0;
  long Idle, Total, NonIdle;

  Idle = CpuData[kIdle_] + CpuData[kIOwait_];
  NonIdle = CpuData[kUser_] + CpuData[kNice_] + CpuData[kSystem_] +
            CpuData[kIRQ_] +CpuData[kSoftIRQ_] +CpuData[kSteal_];

  Total = Idle + NonIdle;
  long TotalDiff = Total - PrevTotal;
  long IdleDiff = Idle - PrevIdle;
  float cpu_calculation = float(TotalDiff-IdleDiff)/float(TotalDiff);
  PrevTotal = Total;
  PrevIdle = Idle;

  result = std::to_string(cpu_calculation);
}
*/

string LinuxParser::CpuUtilization() {
  string line;
  float CPU_Percentage = 0.0f;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  std::ifstream FileReader(kProcDirectory + kStatFilename);
  if (FileReader.is_open()) {
    static long prevTotal = 0, prevIdle = 0;
    long Total, Idle;
    float total_diff, idle_diff;
    std::getline(FileReader, line);
    std::istringstream StringStream(line);
    StringStream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
               softirq >> steal >> guest >> guest_nice;
    Total = std::stol(user) + std::stol(nice) + std::stol(system) +
            std::stol(idle) + std::stol(iowait) + std::stol(irq) +
            std::stol(softirq) + std::stol(steal) + std::stol(guest) +
            std::stol(guest_nice);
    Idle = std::stol(idle) + std::stol(iowait);
    total_diff = (float)(Total - prevTotal);
    idle_diff = (float)(Idle - prevIdle);
    CPU_Percentage = ((total_diff - idle_diff) / total_diff);
    prevTotal = Total;
    prevIdle = Idle;
  }
  return std::to_string(CPU_Percentage);
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  int Count = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        Count = std::stoi(value);
      }
    }
  }
  return Count;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int Count = 0;
  std::ifstream FileReader (kProcDirectory + kStatFilename);
  if (FileReader.is_open()) {
    while (std::getline(FileReader, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        Count = std::stoi(value);
      }
    }
  }
  return Count;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream FileReader(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line, command;

  if (FileReader.is_open()) {
    getline(FileReader, line);
    std::istringstream LineReader (line);
    LineReader >> command;
  } else return "";

  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream FileReader(kProcDirectory + std::to_string(pid) +
                         kStatusFilename);
  double RamKB;
  if (FileReader.is_open()) {
    string line, key, value;
    while(getline(FileReader,line)){
      std::istringstream LineReader(line);
      LineReader >> key >> value;
      if (key == "VmSize:") RamKB = std::stod(value);
    }
  } else return string();

  return to_string((int)RamKB/1024);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream FileReader (kProcDirectory + std::to_string(pid) +
                         kStatusFilename);
  string line, key, value, uid;
  if (FileReader.is_open()) {
    while(getline(FileReader,line)){
      std::istringstream LineReader(line);
      LineReader >> key >> value;
      if (key == "Uid:") uid = value;
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, user, x, uid, result;
  string UID = LinuxParser::Uid(pid);
  std::ifstream FileReader (kPasswordPath);
  if (FileReader.is_open()) {
    while (std::getline(FileReader, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> user >> x >> uid) {
        if (uid == UID)
          result = user;
      }
    }
  }
  return result;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream FileReader(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  string token;
  int count = 0;
  long uptime = 0;
  if(FileReader.is_open()){
    while (getline(FileReader, line)) {
      std::istringstream LineReader(line);
      while(LineReader >> token) {
        ++count;
        if(count == 22) uptime =  std::stol(token)/sysconf(_SC_CLK_TCK);
      }
    }
  }
  return uptime;
}

float LinuxParser::CpuUtilization(int pid) {
  string line, key;
  long utime = 0, stime = 0, cutime = 0, cstime = 0, starttime = 0, uptime = 0,
       Hertz = 0, seconds = 0, total_time = 0;
  int count = 1;
  float cpu_usage;

  std::ifstream FileReader (kProcDirectory + to_string(pid) + kStatFilename);
  if (FileReader.is_open()) {
    while (std::getline(FileReader, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        switch (count) {
          case 14:
            utime = std::stol(key);
            break;
          case 15:
            stime = std::stol(key);
            break;
          case 16:
            cutime = std::stol(key);
            break;
          case 17:
            cstime = std::stol(key);
            break;
          case 22:
            starttime = std::stol(key);
            break;
          default:
            break;
        }
        count++;
      }
    }
  }
  uptime = LinuxParser::UpTime();
  Hertz = sysconf(_SC_CLK_TCK);
  total_time = utime + stime + cutime + cstime;
  seconds = uptime - (long)((float)starttime / (float)Hertz);
  cpu_usage = (((float)total_time / (float)Hertz) / (float)seconds);
  return cpu_usage;
}