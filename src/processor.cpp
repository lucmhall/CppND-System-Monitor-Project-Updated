#include "processor.h"

#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

float Processor::Utilization() {
  float utilization = 0.0f;
  vector<string> proc_stats = LinuxParser::CpuUtilization();

  if (prev_proc_stats.size()) {
    double idle = Idle(proc_stats);
    double non_idle = NonIdle(proc_stats);
    double prev_idle = Idle(prev_proc_stats);
    double prev_non_idle = NonIdle(prev_proc_stats);

    double total = idle + non_idle;
    double prev_total = prev_idle + prev_non_idle;

    double totald = total - prev_total;
    double idled = idle - prev_idle;

    utilization = (totald - idled) / totald;
  }
  prev_proc_stats = proc_stats;

  return 1.0f - utilization;
}

double Processor::NonIdle(const vector<string> proc_stats) {
  return stod(proc_stats[3]) + stod(proc_stats[4]);
}

double Processor::Idle(const vector<string> proc_stats) {
  return stod(proc_stats[0]) + stod(proc_stats[1]) + stod(proc_stats[2]) +
         stod(proc_stats[5]) + stod(proc_stats[6]) + stod(proc_stats[7]);
}
