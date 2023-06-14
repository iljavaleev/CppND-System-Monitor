#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>
class Processor {
public:
    float Utilization();
    Processor();
private:
    long idle;
    long nonIdle;
    long total;
};

#endif
