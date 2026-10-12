#pragma once

#include <string>

// Opens the log file, overwrites any old one
// Returns false if file cant be opened
bool initLog(const std::string& path);

// Writes one timestamped line to the log file
void logLine(const std::string& msg);