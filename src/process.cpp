#include "process.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid(pid) {}

int Process::Pid() { return pid; }

float Process::CpuUtilization() {
  long hz = sysconf(_SC_CLK_TCK);
  long uptime = LinuxParser::UpTime();
  vector<string> proc_stats{};
  string line;
  string value;
  std::ifstream filestream(LinuxParser::kProcDirectory + "/" + to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        proc_stats.push_back(value);
      }
    }

    float utime = std::stof(proc_stats[13]) / hz;
    float stime = std::stof(proc_stats[14]) / hz;
    float cutime = std::stof(proc_stats[15]) / hz;
    float cstime = std::stof(proc_stats[16]) / hz;
    float starttime = std::stof(proc_stats[21]) / hz;

    return (utime + stime + cutime + cstime) / (uptime - starttime);
  }
  return 0;
}

string Process::Command() { return LinuxParser::Command(pid); }

string Process::Ram() { return LinuxParser::Ram(pid); }

string Process::User() { return LinuxParser::User(pid); }

long int Process::UpTime() { return LinuxParser::UpTime(pid); }

bool Process::operator<(Process const& a) const { return pid < a.pid; }