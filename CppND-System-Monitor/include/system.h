#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <map>

#include "process.h"
#include "processor.h"
#include "user.h"

class System {
 public:
  System();
  ~System();
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp
  std::map<std::string, User> GetUsers(){return users_;}
  // TODO: Define any necessary private members
 private:
  void ReadUsers();
  Processor cpu_ ;
  std::vector<Process> processes_ = {};
  std::map<std::string, User> users_;

};

#endif