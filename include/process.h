#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
    Process();
    int getPID() const;
    void setPID(int);
    
    std::string getUser() const;
    void setUser(std::string);
    
    std::string getCMD() const;
    void setCMD(std::string);
    
    float getCPU() const;
    void setCPU(float);
    
    float getMemory() const;
    void setMemory(float);
    
    long int getUpTime() const;
    void setUpTime(long int);
    
    std::string getRam() const;
    bool operator<(Process const& a[[maybe_unused]]) const;
    float CpuUtilization();
    
 private:
    int PID{};
    std::string User{};
    std::string CMD{};
    float CPU{};
    float Memory{};
    long int UpTime{};
    
    
};

#endif
