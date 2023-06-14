#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include "stdlib.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include "dirent.h"
#include "time.h"
#include "unistd.h"
#include "constants.h"
#include "ProcessParser.h"
#include "util.h"


std::vector<std::string> ProcessParser::getStatValues(std::string pid){
    std::string filename = Path::basePath() + pid + "/" + Path::statPath();
    std::ifstream file = Util::getStream(filename);
    std::string line{};
    std::getline(file, line);
    
    std::istringstream buf{line};
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);
    return values;
}


std::string get_content(std::string filename, std::string name){
    std::ifstream file = Util::getStream(filename);
    std::string line{};
    float result{};
    while(std::getline(file, line)){
        if (line.compare(0, name.size(), name) == 0){ //match
            std::istringstream buf{line}; // slicing string
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            
            result = (stof(values[1])/float(1024));
            break;
        }
    }
    return to_string(result);
}



std::string ProcessParser::getCmd(std::string pid){
    std::string filename = Path::basePath() + pid + Path::cmdPath();
    std::ifstream file = Util::getStream(filename);
    std::string line{};
    std::getline(file, line);
    
    return line;
                   
}


std::vector<std::string> ProcessParser::getPidList(){
    std::vector<std::string> vector;
    DIR * dir;
    if(!(dir = opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));
    
    dir = opendir("/proc");
    
    while(dirent * dirp = readdir(dir)){
        if(dirp->d_type != DT_DIR)
            continue;
        
        if (all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c){return std::isdigit(c);}))
            vector.push_back(dirp->d_name);
    }
    
    if(closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    
    return vector;
}


std::string ProcessParser::getVmSize(std::string pid){
    std::string filename = Path::basePath() + pid + Path::statusPath();
    return get_content(filename, "VmSize");
}


std::string ProcessParser::getCpuPercent(std::string pid){
    
    std::vector<string> values{ProcessParser::getStatValues(pid)};
    
        
    float freq{(float)sysconf(_SC_CLK_TCK)};
    float stime{stof(values.at(14))/freq}, cutime{stof(values.at(15))/freq}, starttime{stof(values.at(21))/freq}, cstime{stof(values.at(16))/freq};
    float utime{stof(ProcessParser::getProcUpTime(pid))};
    float uptime{(float) ProcessParser::getSysUpTime()};
    
    
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    double res = 100.0*((total_time/freq)/seconds);
    
    return to_string(res);
}


long int ProcessParser::getSysUpTime(){
    std::string sysUptime = Path::basePath() + Path::upTimePath();
    std::ifstream file = Util::getStream(sysUptime);
    std::string line{};
    std::getline(file, line);
    std::istringstream buf{line};
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);
    return stoi(values.at(0));
}


std::string ProcessParser::getProcUpTime(std::string pid){
    std::vector<string> values{ProcessParser::getStatValues(pid)};
    return to_string(float(stof(values.at(13))/sysconf(_SC_CLK_TCK)));
}



std::string ProcessParser::getProcUser(std::string pid){
    std::string name{"Uid:"};
    std::string filename = Path::basePath() + pid + Path::statusPath();
    string uid{get_content(filename, name)};
    
    std::ifstream file = Util::getStream("/etc/passwd");
    std::string line{}, result{};
    std::size_t found{};
    while(std::getline(file, line)){
        found = line.find(uid);
        if (found != std::string::npos){
            found = line.find(":");
            result = line.substr(0, found);
        }
    }
    return result;
}

int ProcessParser::getNumberOfCores(){
    std::string filename = "/proc/cpuinfo";
    std::ifstream file = Util::getStream(filename);
    std::string line{}, name{"cpu cores"};
    
    while(std::getline(file, line)){
        if (line.compare(0, name.size(), name) == 0){
            std::istringstream buf{line};
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            
            return stoi(values[3]);
        }
    }
    
    return 0;
}



std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber){
    std::string filename = Path::basePath() + Path::statPath();
    std::ifstream file = Util::getStream(filename);
    std::string line{}, name{"cpu"+coreNumber};
    while(std::getline(file, line)){
        if (line.compare(0, name.size(), name) == 0){
            std::istringstream buf{line};
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            values.erase(values.begin());
            return values;
        }
    }
    
    return std::vector<string>();
}


float getSysActiveCpuTime(vector<string> values){
    return (stof(values[S_USER])
            +stof(values[S_NICE])
            +stof(values[S_SYSTEM])
            +stof(values[S_IRQ])
            +stof(values[S_SOFTIRQ])
            +stof(values[S_STEAL])
            +stof(values[S_GUEST])
            +stof(values[S_GUEST_NICE]));
}

float getSysIdleCpuTime(vector<string> values){
    return (stof(values[S_IDLE])+stof(values[S_IOWAIT]));
}

float ProcessParser::getSysRamPercent(){
    std::string name1{"MemAvailable:"}, name2{"MemFree"}, name3{"Buffers"};
    float free_mem{}, total_mem{}, buffers{};
    
    std::ifstream file = Util::getStream(Path::basePath()+Path::memInfoPath());
    std::string line{};
    
    while(std::getline(file, line)){
        if (line.compare(0, name1.size(), name1) == 0){
            std::istringstream buf{line};
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            total_mem = stof(values.at(1));
        }else if (line.compare(0, name2.size(), name2) == 0){
            std::istringstream buf{line};
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            free_mem = stof(values.at(1));
            
        }else if(line.compare(0, name3.size(), name3) == 0){
            std::istringstream buf{line};
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            buffers = stof(values.at(1));
        }
    }
    return float(100.0*(1-(free_mem/(total_mem-buffers))));
}

std::string ProcessParser::getSysKernelVersion(){
    std::string name{"Linux version "}, line;
    std::ifstream file = Util::getStream(Path::basePath()+Path::versionPath());
    
    while(std::getline(file, line)){
        if (line.compare(0, name.size(), name) == 0){
            std::istringstream buf{line};
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            return values.at(2);
        }
        
    }
    return "";
}

int ProcessParser::getTotalThreads(){
    int sum{};
    std::string name{"Threads:"}, line;
    for(auto pid: getPidList()){
        std::ifstream file = Util::getStream(Path::basePath()+pid+Path::statusPath());
        while(std::getline(file, line)){
            if (line.compare(0, name.size(), name) == 0){
                std::istringstream buf{line};
                std::istream_iterator<string> beg(buf), end;
                std::vector<string> values(beg, end);
                sum += stoi(values.at(1));
            }
            
        }
    }
    return sum;
}

int get_total(std::string name){
    int sum{};
    std::string line;
    
    std::ifstream file = Util::getStream(Path::basePath()+Path::statPath());
    
    while(std::getline(file, line)){
        if (line.compare(0, name.size(), name) == 0){
            std::istringstream buf{line};
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            sum += stoi(values.at(1));
        }
        
    }
    return sum;
}

int ProcessParser::getTotalNumberOfProcesses(){
    std::string name{"processes"};
    return get_total(name);
}

int ProcessParser::getNumberOfRunningProcesses(){
    std::string name{"procs_running"};
    return get_total(name);
}

std::string ProcessParser::getOsName(){
    std::string line, name{"PRETTY_NAME="};
    std::ifstream file = Util::getStream("/etc/os-release");
    std::size_t found{};
    while(std::getline(file, line)){
        found = line.find(name);
        if (found != std::string::npos){
            found = line.find("=");
            return line.substr(found+1);
        }
    }
    return "";
}

std::string ProcessParser::printCpuStats(std::vector<std::string> values1, std::vector<std::string> values2){
    float activeTime{getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1)};
    float idleTime{getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1)};
    float totalTime{activeTime + idleTime};
    
    return to_string(100.0 * activeTime/totalTime);
}

        

