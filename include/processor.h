#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();

 private:
  vector<string> prev_proc_stats;
  double NonIdle(const vector<string> proc_stats);
  double Idle(const vector<string> proc_stats);
};

#endif