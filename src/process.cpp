#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"


Process::Process(){}

int Process::getPID() const{
    return PID;
}

void Process::setPID(int pid){
    PID = pid;
}

std::string Process::getUser() const{
    return User;
}

std::string Process::getCMD() const{
    return CMD;
}

void Process::setUser(std::string user){
    User = user;
}

void Process::setCMD(std::string cmd){
    CMD = cmd;
}

float Process::getCPU() const{
    return CPU;
}

void Process::setCPU(float cpu){
    CPU = cpu;
}

float Process::getMemory() const{
    return Memory;
}

void Process::setMemory(float mem){
    Memory = mem;
}

long int Process::getUpTime() const{
    return UpTime;
}

void Process::setUpTime(long int time){
    UpTime = time;
}

std::string Process::getRam() const { return std::string(); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }

float Process::CpuUtilization(){
    return 0;
}
