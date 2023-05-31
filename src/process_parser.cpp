#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include "stdlib.h"
#include "stdio.h"
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

bool check_number(std::string str) {
   for (size_t i = 0; i < str.length(); i++)
   {
       if (isdigit(str[i]) == false)
           return false;
       
   }
    return true;
}

std::string get_content(std::string filename, std::string name){
    std::ifstream file = Util::getStream(filename);
    std::string line{}, value{};
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
    std::string path{};
    for (const auto & entry: std::filesystem::directory_iterator(path)){
        if (check_number(entry.path().string()))
            vector.push_back(entry.path().string());
        
    }
    
    return vector;
}


std::string ProcessParser::getVmSize(std::string pid){
    std::string filename = Path::basePath() + pid + Path::statusPath();
    return get_content(filename, "VmSize");
}


std::string ProcessParser::getCpuPercent(std::string pid){
    std::string filename = Path::basePath() + pid + Path::statPath();
    std::ifstream file = Util::getStream(filename);
    std::string line{};
    std::getline(file, line);
    
    std::istringstream buf{line};
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);
    
    float freq{(float)sysconf(_SC_CLK_TCK)};
    float uptime{stof(values.at(14))/freq}, stime{stof(values.at(15))/freq}, starttime{stof(values.at(21))/freq}, cstime{stof(values.at(16))/freq};
    
    
    // stop here getSysUpTime
    std::string sysUptime = Path::basePath() + Path::upTimePath();
    file = Util::getStream(sysUptime);
    std::getline(file, line);
    std::istringstream buf1{line};
    std::istream_iterator<string> beg1(buf1), end1;
    std::vector<string> values1(beg, end);
    float sysUp{stof(values1.at(0))};
    
    double res{(uptime + stime) * 100 / (sysUp - starttime)};
    
    return to_string(res) + "%";
}


long int ProcessParser::getSysUpTime(){
    return 0;
}


std::string ProcessParser::getProcUpTime(std::string pid){
    return string();
}



std::string ProcessParser::getProcUser(std::string pid){
    return string();
}

std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber = ""){
    std::vector<std::string> v;
    return v;
}

float ProcessParser::getSysRamPercent(){
    return 0;
}

std::string ProcessParser::getSysKernelVersion(){
    return string();
}

int ProcessParser::getTotalThreads(){
    return 0;
}

int ProcessParser::getTotalNumberOfProcesses(){
    return 0;
}

int ProcessParser::getNumberOfRunningProcesses(){
    return 0;
}

std::string ProcessParser::getOsName(){
    return string();
}

std::string printCpuStats(std::vector<std::string> values1, std::vector<std::string> values2){
    return string();
}

        

