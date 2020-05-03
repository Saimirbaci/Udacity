#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "user.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(){
    ReadUsers();
}

System::~System(){    
}  

void System::ReadUsers(){
    string line;
    std::string name;
    std::string password;
    std::string userID;
    std::string groupID;
    std::string gecos;
    std::string homedirectory;
    std::string shell;
    std::ifstream passwd_stream(LinuxParser::kPasswordPath);
    if(passwd_stream.is_open()){
        while (std::getline(passwd_stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> name >> password >> userID >> groupID >> gecos >> homedirectory >> shell; 
            auto new_user = User(name, password, userID, groupID, gecos, homedirectory, shell);
            users_.insert(std::pair<std::string, User>(userID, new_user));
        }
    }
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    auto pids = LinuxParser::Pids();
    for(auto v : pids){
        auto uid = LinuxParser::Uid(v);
        string username = users_[uid].GetUserName(); 
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