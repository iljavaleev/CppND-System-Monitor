#include <stdio.h>
#include "ProcessParser.h"
#include "SysInfo.h"
#include "util.h"

SysInfo::SysInfo():OsName{ProcessParser::getOsName()}, kernelVer{ProcessParser::getSysKernelVersion()}{
    getOtherCores(ProcessParser::getNumberOfCores());
    setLastCpuMeasures();
    setAttributes();
}

void SysInfo::getOtherCores(int _size){
    coresStats.resize(_size);
    lastCpuCoresStats.resize(_size);
    currentCpuStats.resize(_size);
    for(int i{}; i<_size; i++)
        lastCpuCoresStats.at(i) = ProcessParser::getSysCpuPercent(std::to_string(i));
}

void SysInfo::setCpuCoresStats(){
    for(std::size_t i{}; i < currentCpuStats.size(); i++)
        currentCpuCoresStats.at(i) = ProcessParser::getSysCpuPercent(std::to_string(i));
    for(std::size_t i{}; i < currentCpuStats.size(); i++)
        coresStats.at(i) = ProcessParser::printCpuStats(lastCpuCoresStats.at(i), currentCpuCoresStats.at(i));
    lastCpuCoresStats = currentCpuCoresStats;
}

void SysInfo::setAttributes(){
    memPercent = ProcessParser::getSysRamPercent();
    upTime = ProcessParser::getSysUpTime();
    totalProc = ProcessParser::getTotalNumberOfProcesses();
    runningProc = ProcessParser::getNumberOfRunningProcesses();
    threads = ProcessParser::getTotalThreads();
    currentCpuStats = ProcessParser::getSysCpuPercent();
    lastCpuStats = currentCpuStats;
    cpuPercent = ProcessParser::printCpuStats(lastCpuStats, currentCpuStats);
    setCpuCoresStats();
}

vector<string> SysInfo::getCoresStats() const{
    vector<string> result;
    for (std::size_t i{}; i<coresStats.size(); i++){
        string temp = "cpu"+std::to_string(i)+": ";
        
        if (stof(coresStats.at(i)) || coresStats.at(i) == "nan")
            return vector<string> ();
        
        temp += Util::getProgressBar(coresStats.at(i));
        
        result.push_back(temp);
    }
    return result;
}

string SysInfo::getCpuPercent() const
{
    return cpuPercent;
}

string SysInfo::getMemPercent() const
{
    return std::to_string(memPercent);
}

long SysInfo::getUpTime() const
{
    return upTime;
}

string SysInfo::getKernelVersion() const
{
    return kernelVer;
}

string SysInfo::getTotalProc() const
{
    return std::to_string(totalProc);
}

string SysInfo::getRunningProc() const
{
    return std::to_string(runningProc);
}

string SysInfo::getThreads() const
{
    return std::to_string(threads);
}

string SysInfo::getOsName() const
{
    return OsName;
}

void SysInfo::setLastCpuMeasures()
{
    lastCpuStats = ProcessParser::getSysCpuPercent();
}
