#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <thread> 

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, std::string user, std::string command):  user_(user), pid_(pid), command_(command){

}
// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{   
  return cpu_utilization_;
}

void Process::Stats(){
  string line;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  std::getline(filestream, line);
  std::istringstream linestream(line);
  int pid;
  string command;
  char state;
  linestream >> pid >> command >> state >> ppid_ >> pgrp_ >> session_ >> tty_nr_ >> tpgid_ >> flags_ >> 
          minflt_ >> cminflt_ >> majflt_ >> cmajflt_ >> utime_ >> stime_ >> cutime_ >> cstime_ >> priority_ >> 
          nice_ >> num_threads_ >> itrealvalue_ >> starttime_ >> vsize_ >> rss_ >> rsslim_ >> startcode_ >> 
          endcode_ >> startstack_ >> kstkesp_ >> kstkeip_ >> signal_ >> blocked_ >> sigignore_ >> sigcatch_ >> 
          wchan_ >> nswap_ >> cnswap_ >> exit_signal_ >> processor_ >> rt_priority_ >> policy_ >> 
          delayacct_blkio_ticks_ >> guest_time_ >> cguest_time_ >> start_data_ >> end_data_ >> 
          start_brk_ >> arg_start_ >> arg_end_ >> env_start_ >> env_end_ >> exit_code_;

  long totaltime = utime_ + stime_ + cutime_ + cstime_;
  float uptime = LinuxParser::UpTime();                 // In seconds
  float secondsactive =
      uptime - (starttime_ / sysconf(_SC_CLK_TCK));  // In seconds
  float cpu_usage =
      (totaltime / sysconf(_SC_CLK_TCK)) / secondsactive;  // In seconds
  
  cpu_utilization_ = cpu_usage;
}
// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() {
  return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const {
  return starttime_ / sysconf(_SC_CLK_TCK);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return CpuUtilization() < a.CpuUtilization(); 
}

int Process::Processor(){
  return processor_;
};

unsigned long Process::Threads(){
  return num_threads_;
};

