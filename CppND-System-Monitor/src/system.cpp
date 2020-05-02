#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
    auto pids = LinuxParser::Pids();
    for(auto v : pids){
        string line;
        string username = LinuxParser::User(v);
        string command_name = LinuxParser::Command(v);

        Process process(v, username, command_name);
        processes_.push_back(process);
    }
}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    auto pids = LinuxParser::Pids();
    for(auto v : pids){
        string line;
        string username = LinuxParser::User(v);
        string command_name = LinuxParser::Command(v);

        Process process(v, username, command_name);
        process.Stats();
        processes_.push_back(process);
    }
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }