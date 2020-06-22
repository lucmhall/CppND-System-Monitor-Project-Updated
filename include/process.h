#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  void Update();

 private:
  int pid;
  std::string command;
  std::string user;
  float cpu_utilization;
  std::string ram;
  long int uptime;
  float CalculateUtilization();
};

#endif