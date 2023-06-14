#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "ProcessParser.h"
using std::string;

Process::Process(string pid):PID{pid}, User{ProcessParser::getProcUser(pid)}, CMD(ProcessParser::getCmd(pid)), CPU(ProcessParser::getCpuPercent(pid)), Memory(ProcessParser::getVmSize(pid)), UpTime(ProcessParser::getProcUpTime(pid)){}

string Process::getPID() const{
    return PID;
}

void Process::setPID(string pid){
    PID = pid;
}

string Process::getUser() const{
    return User;
}

string Process::getCMD() const{
    return CMD;
}

void Process::setUser(std::string user){
    User = user;
}

void Process::setCMD(std::string cmd){
    CMD = cmd;
}

string Process::getCPU() const{
    return CPU;
}

void Process::setCPU(string cpu){
    CPU = cpu;
}

string Process::getMemory() const{
    return Memory;
}

void Process::setMemory(string mem){
    Memory = mem;
}

string Process::getUpTime() const{
    return UpTime;
}

void Process::setUpTime(string time){
    UpTime = time;
}

std::string Process::getRam() const { return std::string(); }

bool Process::operator<(Process const& a) const {
    return stol(Memory) + stol(CPU) < stol(a.getMemory()) + stol(a.getCPU());
}

float Process::CpuUtilization(){
    return 0;
}

string Process::getProcess(){
    return PID + " " + User + " " + Memory.substr(0, 5) + " " + CPU.substr(0, 5) + " " + UpTime.substr(0, 5) + " " + CMD.substr(0, 30) + "...";
}
