#include "processor.h"
#include "linux_parser.h"


using std::stoi;

Processor::Processor():idle(LinuxParser::IdleJiffies()), nonIdle(LinuxParser::ActiveJiffies()), total(idle+nonIdle){}

float Processor::Utilization() {
    long currentIdle{LinuxParser::IdleJiffies()},currentNonIdle{LinuxParser::ActiveJiffies()};
    long currentTotal = currentIdle + currentNonIdle;
    
    long deltaIdle = currentIdle - idle;
    long deltaTotal = currentTotal - total;
    
    idle = currentIdle;
    nonIdle = currentNonIdle;
    total = currentTotal;
    
    return float(deltaTotal - deltaIdle)/float(deltaTotal);
}
