#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

class Process {
 public:
    Process(string);
    string getPID() const;
    void setPID(string);
    
    string getUser() const;
    void setUser(string);
    
    string getCMD() const;
    void setCMD(string);
    
    string getCPU() const;
    void setCPU(string);
    
    string getMemory() const;
    void setMemory(string);
    
    string getUpTime() const;
    void setUpTime(string);
    
    string getRam() const;
    bool operator<(Process const& a) const;
    float CpuUtilization();
    
    string getProcess();
    
 private:
    string PID{};
    string User{};
    string CMD{};
    string CPU{};
    string Memory{};
    string UpTime{};
    
    
};

#endif
