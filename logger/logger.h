#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

class Logger {
public:
    static void init() {

    }

    static void log(const std::string& message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    static void logError(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }
};

#endif

