#include "util.h"

#include <stdio.h>
#include <string>
#include <fstream>

std::string Util::convertToTime(long int input_seconds){
    long minuts = input_seconds / 60;
    long hours = minuts / 60;
    long seconds = int(minuts%60);
    minuts = int(minuts%60);
    
    std::string result = std::to_string(hours) + ":" + std::to_string(minuts) + ":" + std::to_string(seconds);
    return result;
}


std::string Util::getProgressBar(std::string percent){
    std::string result = "0% ";
    int _size = 50;
    int boundaries = (stof(percent) / 100) * _size;
    
    for (int i{}; i < _size; i++){
        if (i <= boundaries){
            result += "|";
        }else{
            result += " ";
        }
    }
    
    result += " " + percent.substr(0, 5) + " /100%";
    return result;
}


std::ifstream Util::getStream(std::string path){
    std::ifstream stream(path);
    if (!stream){
        throw std::runtime_error("PID doesn'n exist");
    }
    return stream;
}