//
// Created by ichigo on 29.10.2020.
//

#ifndef GMW_TESTS_MAINTENANCE_HPP
#define GMW_TESTS_MAINTENANCE_HPP

#include <iostream>
#include <string>

namespace {
#define TEST_START_SEGREGATION "//"
#define TEST_FINISH_SEGREGATION "\\\\"

    std::string lastTestName;

    std::string greenColoredBold(std::string string);

    std::string yellowColored(std::string string);

    std::string yellowColoredBold(std::string string);

    std::string blueColored(std::string string);

    std::string blueColoredBold(std::string string);

    void printTestStarted(std::string testName) {
        lastTestName = testName;
        std::cout << blueColoredBold(TEST_START_SEGREGATION) <<
                  blueColored(" Started  : ") <<
                  greenColoredBold(lastTestName) <<
                  blueColored(" test ") <<
                  blueColoredBold(TEST_START_SEGREGATION) << std::endl;
    }

    void printLastTestFinished() {
        std::cout << blueColoredBold(TEST_FINISH_SEGREGATION) <<
                  blueColored(" Finished : ") <<
                  greenColoredBold(lastTestName) <<
                  blueColored(" test ") <<
                  blueColoredBold(TEST_FINISH_SEGREGATION) << std::endl << std::endl;
    }

    void printTODO(std::string description) {
        std::cout << yellowColoredBold("(!) TODO : ") <<
                  yellowColored(description) <<
                  yellowColoredBold(" (!)") << std::endl;
    }

    std::string colored(std::string &, int);

    std::string coloredBold(std::string &, int);

    std::string greenColoredBold(std::string string) {
        return coloredBold(string, 32);
    }

    std::string yellowColoredBold(std::string string) {
        return coloredBold(string, 33);
    }

    std::string yellowColored(std::string string) {
        return colored(string, 33);
    }

    std::string blueColored(std::string string) {
        return colored(string, 34);
    }

    std::string blueColoredBold(std::string string) {
        return coloredBold(string, 34);
    }

    std::string colored(std::string &string, int colorCode) {
        return "\033[" + std::to_string(colorCode) + "m" + string + "\033[m";
    }

    std::string coloredBold(std::string &string, int colorCode) {
        return "\033[1;" + std::to_string(colorCode) + "m" + string + "\033[m";
    }
}

#endif //GMW_TESTS_MAINTENANCE_HPP
