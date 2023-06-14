#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::to_string;
using std::vector;


std::string LinuxParser::OperatingSystem() {
  std::string line, key, value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

std::string LinuxParser::Kernel() {
  std::string os, kernel, version;
  std::string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR* dir = opendir(kProcDirectory.c_str());

    struct dirent* dirp;
    while ((dirp = readdir(dir)) != nullptr) {
    
        if (dirp->d_type == DT_DIR) {
          
            std::string filename{dirp->d_name};
          if (std::all_of(filename.begin(), filename.end(), isdigit)) {
            int pid = stoi(filename);
            pids.push_back(pid);
          }
        }
    }
    closedir(dir);
    return pids;
}

float LinuxParser::MemoryUtilization() {
    std::string name1{"MemAvailable:"}, name2{"MemFree"}, name3{"Buffers"};
    float free_mem{}, total_mem{}, buffers{};
    
    std::ifstream file(kProcDirectory + kMeminfoFilename);
    std::string line{};
    if (file.is_open()){
        while(std::getline(file, line)){
            if (line.compare(0, name1.size(), name1) == 0){
                std::istringstream buf{line};
                std::istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> values(beg, end);
                total_mem = stof(values.at(1));
            }else if (line.compare(0, name2.size(), name2) == 0){
                std::istringstream buf{line};
                std::istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> values(beg, end);
                free_mem = stof(values.at(1));
                
            }else if(line.compare(0, name3.size(), name3) == 0){
                std::istringstream buf{line};
                std::istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> values(beg, end);
                buffers = stof(values.at(1));
            }
        }
        
    }
    return float(100.0*(1-(free_mem/(total_mem-buffers))));
}

long LinuxParser::UpTime() {
    std::string sysUptime = kProcDirectory + kUptimeFilename;
    std::ifstream file(sysUptime);
    if(file.is_open()){
        std::string line{};
        std::getline(file, line);
        std::istringstream buf{line};
        std::istream_iterator<std::string> beg(buf), end;
        std::vector<std::string> values(beg, end);
        return stol(values.at(0));
    }
    return -1;
}


std::vector<std::string> getStatValues(int pid){
    std::string filename = LinuxParser::kProcDirectory + to_string(pid) + "/" + LinuxParser::kStatFilename;
    std::ifstream file(filename);
    std::string line{};
    if(file.is_open()){
        std::getline(file, line);
        
        std::istringstream buf{line};
        std::istream_iterator<std::string> beg(buf), end;
        std::vector<std::string> values(beg, end);
        return values;
    }
    return {};
}


long LinuxParser::Jiffies() {
    return ActiveJiffies() + IdleJiffies();
}


long LinuxParser::ActiveJiffies(int pid) {
    std::vector<std::string> values{getStatValues(pid)};
    float freq{(float) sysconf(_SC_CLK_TCK)};
    float stime{stof(values.at(14))/freq}, cutime{stof(values.at(15))/freq}, cstime{stof(values.at(16))/freq};
    float utime{stof(values.at(13))/freq};
    float totaltime = utime + stime + cutime + cstime;
    
    return long(totaltime);
}


long LinuxParser::ActiveJiffies() {
    auto jiffies = CpuUtilization();

    return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
}


long LinuxParser::IdleJiffies() {
    auto jiffies = CpuUtilization();
    
    return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
}


vector<std::string> LinuxParser::CpuUtilization() {
    // /proc/stat first line
    std::string line, cpu, value;
    vector<std::string> jiffies;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);

      linestream >> cpu;

      while (linestream >> value) {
        jiffies.push_back(value);
      }
    }
    return jiffies;
    
}

int get_total(std::string name){
    int sum{};
    std::string line;
    
    std::ifstream file(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if(file.is_open()){
        while(std::getline(file, line)){
            if (line.compare(0, name.size(), name) == 0){
                std::istringstream buf{line};
                std::istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> values(beg, end);
                sum += stoi(values.at(1));
            }
            
        }
        
    }
    return sum;
}


int LinuxParser::TotalProcesses() {
    return get_total("processes");
}


int LinuxParser::RunningProcesses() {
    return get_total("procs_running");
}


std::string LinuxParser::Command(int pid) {
    std::string filename{kProcDirectory + to_string(pid) + kCmdlineFilename}, line{};;
    std::ifstream file(filename);
    if(file.is_open()){
        std::getline(file, line);
    }
    return line;
    
}


std::string get_content(std::string filename, std::string name){
    std::ifstream file(filename);
    std::string line{};
    float result{};
    if(file.is_open()){
        while(std::getline(file, line)){
            if (line.compare(0, name.size(), name) == 0){ //match
                std::istringstream buf{line}; // slicing string
                std::istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> values(beg, end);
                
                result = (stof(values[1])/float(1024));
                break;
            }
        }
        
    }
    return to_string(result);
}


std::string LinuxParser::Ram(int pid) {
    std::string filename{kProcDirectory + to_string(pid) + kStatusFilename};
    return get_content(filename, "VmSize");
    
}


std::string LinuxParser::Uid(int pid) {
    std::string name{"Uid:"};
    std::string filename{kProcDirectory + to_string(pid) + kStatusFilename};
    return get_content(filename, name);
}


std::string LinuxParser::User(int pid) {
    std::ifstream file("/etc/passwd");
    std::string line{}, result{}, uid{Uid(pid)};
    std::size_t found{};
    if(file.is_open()){
        while(std::getline(file, line)){
            found = line.find(uid);
            if (found != std::string::npos){
                found = line.find(":");
                result = line.substr(0, found);
            }
        }
    }
    return result;
}

long LinuxParser::UpTime(int pid) {
    std::vector<std::string> values{getStatValues(pid)};
    return long(float(stof(values.at(13))/sysconf(_SC_CLK_TCK)));
}
