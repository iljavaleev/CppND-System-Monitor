#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>


#include "process.h"
#include "processor.h"

class System {
public:
    Processor& get_cpu();
    std::vector<Process>& get_processes();
    
    
    std::string get_kernel();
    
    float MemoryUtilization();
    long UpTime();
    int TotalProcesses();
    int RunningProcesses();
    
    std::string OperatingSystem();

  
 private:
    Processor cpu_{};
    std::vector<Process> processes_;
};

#endif
