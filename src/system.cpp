#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// TODO: Optimize
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  vector<int> existing_pids{};
  for (Process process : processes_) {
    existing_pids.push_back(process.Pid());
  }

  for (int pid : pids) {
    if (std::find(existing_pids.begin(), existing_pids.end(), pid) ==
        existing_pids.end()) {
      processes_.push_back(Process(pid));
    }
  }

  for (Process process : processes_) {
    process.Update();
  }

  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }