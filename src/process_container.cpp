#include <stdio.h>
#include "ProcessCon.h"
#include "ProcessParser.h"

ProcessContainer::ProcessContainer(){
    refreshList();
}

void ProcessContainer::refreshList(){
    list.clear();
    std::vector<std::string> pids = ProcessParser::getPidList();
    for (auto it = pids.cbegin(); it != pids.cend(); ++it){
        Process tmp{*it};
        list.push_back(tmp);
    }
}

std::string ProcessContainer::printList(){
    std::string res;
    for (std::size_t i{}; i<list.size(); ++i)
        res += list.at(i).getProcess() + " ";
    return res;
}

std::vector<std::string> ProcessContainer::getList(){
    std::vector<std::string> vals;
    for (std::size_t i{list.size() - 10}; i<list.size(); ++i)
        vals.push_back(list.at(i).getProcess());
    return vals;
}

