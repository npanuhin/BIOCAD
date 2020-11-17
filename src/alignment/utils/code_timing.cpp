#include "code_timing.hpp"

#include <iostream>
#include <ctime>

long int timeStart = -1, timeEnd;

void startTiming() {
    std::cout << "Code timing start" << std::endl;
    timeStart = clock();
}

void finishTiming() {
    if (timeStart == -1) {
        std::cerr << "Code timing error: you've not started timing" << std::endl;
    } else {
        timeEnd = clock();
        std::cout << "Code timing : " << ((timeEnd - timeStart) / 1000000.0) << std::endl;
    }
}
