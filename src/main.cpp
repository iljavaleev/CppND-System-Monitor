#include "ncurses_display.h"
#include "system.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "util.h"

bool check_number(std::string str) {
   for (size_t i = 0; i < str.length(); i++)
   {
       if (!isdigit(str[i]))
           return false;
       
   }
    return true;
}


int main() {
    System system;
    NCursesDisplay::Display(system);
    
    return 0;
}
