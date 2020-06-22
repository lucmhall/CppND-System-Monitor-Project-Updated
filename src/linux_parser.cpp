#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memory_used = 0.0f;
  float memory_total;
  float memory_free;

  // memtotal - memfree / memtotal
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memory_total = stoi(value);
        }
        if (key == "MemFree") {
          memory_free = stoi(value);
        }
      }
    }
  }

  if (memory_total && memory_free) {
    memory_used = ((memory_total - memory_free) / memory_total);
  }

  return memory_used;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;

  // memtotal - memfree / memtotal
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() {
  string line;
  vector<string> proc_stats{};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  string value;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    // read off first value, its 'cpu'
    linestream >> value;
    while (linestream >> value) {
      proc_stats.push_back(value);
    }
  }
  return proc_stats;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_processes;

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "processes") {
          total_processes = stoi(value);
        }
      }
    }
  }

  return total_processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int total_processes;

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "procs_running") {
          total_processes = stoi(value);
        }
      }
    }
  }

  return total_processes;
}

string LinuxParser::Command(int pid) {
  string line;
  string command;
  string file_name = LinuxParser::kProcDirectory + std::to_string(pid) +
                     LinuxParser::kCmdlineFilename;
  std::ifstream filestream(file_name);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}

string LinuxParser::Ram(int pid) {
  string size;
  string line;
  string key;
  string file_name = LinuxParser::kProcDirectory + std::to_string(pid) +
                     LinuxParser::kStatusFilename;
  std::ifstream filestream(file_name);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> size;
      if (key == "VmSize") {
        return to_string(stoi(size) / 1024);
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid [[maybe_unused]]) {
  string uid;
  string line;
  string key;
  string file_name = LinuxParser::kProcDirectory + std::to_string(pid) +
                     LinuxParser::kStatusFilename;
  std::ifstream filestream(file_name);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid") {
        return uid;
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string username;
  string line;
  string key;
  string x;
  string line_uid;
  string file_name = LinuxParser::kPasswordPath;

  std::ifstream filestream(file_name);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> line_uid;
      if (line_uid == uid) {
        return username;
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  vector<string> line_contents;
  string file_name = LinuxParser::kProcDirectory + std::to_string(pid) +
                     LinuxParser::kStatFilename;
  std::ifstream filestream(file_name);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      line_contents.push_back(value);
    }
  }
  return LinuxParser::UpTime() -
         (stol(line_contents[21]) / sysconf(_SC_CLK_TCK));
}