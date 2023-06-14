#ifndef ProcessCon_h
#define ProcessCon_h
#include "process.h"
#include <vector>
#include <string>
class ProcessContainer{
private:
    std::vector<Process> list;
public:
    ProcessContainer();
    void refreshList();
    std::string printList();
    std::vector<std::string> getList();
    
};

#endif /* ProcessCon_h */
