#ifndef SysInfo_h
#define SysInfo_h
#include <string>
#include <vector>

using std::string;
using std::vector;

class SysInfo{

private:
    
    
    vector<string> coresStats;
    vector<string> lastCpuStats;
    vector<string> currentCpuStats;
    
    vector<vector<string>> lastCpuCoresStats;
    vector<vector<string>> currentCpuCoresStats;
    
    string cpuPercent;
    float memPercent{};
    string OsName;
    string kernelVer;
    long upTime{};
    int totalProc{};
    int runningProc{};
    int threads{};

public:
    SysInfo();
    
    void setAttributes();
    void setLastCpuMeasures();
    void getOtherCores(int _size);
    void setCpuCoresStats();
    
    
    vector<string> getCoresStats() const;
    string getCpuPercent() const;
    string getMemPercent() const;
    string getOsName() const;
    string getKernelVersion() const;
    long getUpTime() const;
    string getTotalProc() const;
    string getRunningProc() const;
    string getThreads() const;

};

#endif /* SysInfo_h */
