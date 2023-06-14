#ifndef ProcessParser_h
#define ProcessParser_h
#include <string>
#include <vector>

class ProcessParser {
    public:
        static std::string getCmd(std::string pid);
        static std::vector<std::string> getPidList();
        static std::string getVmSize(std::string pid);
        static std::string getCpuPercent(std::string pid);
        static long int getSysUpTime();
        static std::string getProcUpTime(std::string pid);
        static std::string getProcUser(std::string pid);
        static std::vector<std::string> getSysCpuPercent(std::string coreNumber="");
        static float getSysRamPercent();
        static std::string getSysKernelVersion();
        static int getTotalThreads();
        static int getTotalNumberOfProcesses();
        static int getNumberOfRunningProcesses();
        static std::string getOsName();
        static std::string printCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
    
        
        static int getNumberOfCores();
        static std::vector<std::string> getStatValues(std::string);

};

#endif
