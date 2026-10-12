#include "logger.h"
#include "protocol.h" // timestamp()

#include <fstream>
#include <iostream>

static std::ofstream logFile; // one file for the whole program

bool initLog(const std::string& path) {
    logFile.open(path); // default mode, overwrites file each server start
    return logFile.is_open();
}

void logLine(const std::string& msg) {
    std::string line = timestamp() + " " + msg;
    std::cout << line << '\n';
    if (logFile) {
        // endl flushes, so if server crashes the log is ok
        logFile << line << std::endl;
    }
}
