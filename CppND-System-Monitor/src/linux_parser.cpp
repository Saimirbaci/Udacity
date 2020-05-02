#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  long total_mem = 1;
  long available = 0;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      string key;
      long value;
      string unit;
      
      //std::replace(line.begin(), line.end(), ' ', '_');
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      if(key.compare("MemTotal:") == 0){
        total_mem = value;
      }
      if(key.compare("MemAvailable:") == 0){
        available = value;
      }
    }
  }
  return (float(total_mem-available)/float(total_mem));
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  float uptime = 1;
  float from_start = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime >> from_start ;
    }
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { 
  return 0;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  //long totaltime = 0;
  string line;
  vector<string> cpu_info;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      string cpu;
      //unsigned long long int systemaltime, idlealltime, totaltime, virtalltime;
      unsigned long long int usertime, nicetime, systemtime, idletime, ioWait, irq, softIrq, steal, guest, guestnice;
      
      std::istringstream linestream(line);
      linestream >> cpu >> usertime >> nicetime>> systemtime >> idletime >> ioWait >> irq >> softIrq >> steal >> guest >> guestnice;
      
      if(cpu.compare("cpu") == 0){
        cpu_info.push_back(line);
      }
      else if((cpu.size() == 4) && (cpu.find("cpu") != std::string::npos)){
        cpu_info.push_back(line);
      }
    }
  }
 
  return cpu_info; 
  
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {   
  long total_processes = 0;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      string key;
      long value;

      string unit;
      
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key.compare("processes") == 0){
        total_processes = value;
      }
    }
  }
  return total_processes;
  
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string line;
  long running_processes = 0;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      string key;
      long value;
      string unit;
      
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key.compare("procs_running") == 0){
        running_processes = value;
      }
    }
  }
  return running_processes;
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
    string command_name;
    std::ifstream commandstream(LinuxParser::kProcDirectory + std::to_string(pid) + "/comm");
    if(commandstream.is_open()){
            commandstream >> command_name;
    }
  return command_name; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream statusstream(LinuxParser::kProcDirectory + std::to_string(pid)+ "/status");
  string line;
  float vmsize;
  if(statusstream.is_open()){
    while (std::getline(statusstream, line)) {
        string uid_line;
        std::istringstream linestream(line);
        linestream >> uid_line; 
        if(uid_line.compare("VmSize:")){
            linestream >> vmsize;

        }
    }
  }
  return to_string(vmsize/1024);
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream statusstream(LinuxParser::kProcDirectory + std::to_string(pid)+ "/status");
  string line;
  string uid;
  if(statusstream.is_open()){
    while (std::getline(statusstream, line)) {
        string uid_line;
        std::istringstream linestream(line);
        linestream >> uid_line; 
        if(uid_line.compare("Uid:")){
            linestream >> uid;
        }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string username;//=std::to_string(pid);
  std::ifstream passwd_stream(kPasswordPath);
  if(passwd_stream.is_open()){
      string uid_str = LinuxParser::Uid(pid);

      while (std::getline(passwd_stream, line)) {
          string uid_line;
          std::istringstream linestream(line);
          linestream >> uid_line; 
          if(uid_line.find(uid_str) != std::string::npos){
              username = line.substr(0, line.find(":"));
          }
      }
  }
  return username;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { 
  string line;
  string var;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for(int i = 1; i < 23; i++) {
        linestream >> var; // Grab the 22nd value
      }
      return stol(var) / sysconf(_SC_CLK_TCK); // Return uptime in seconds
    }
  }
return -1;
}