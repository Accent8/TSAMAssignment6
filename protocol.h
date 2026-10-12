#pragma once

#include <string>

constexpr char SOH = 0x01;
constexpr char STX = 0x02;
constexpr char ETX = 0x03;
constexpr size_t MAX_FRAME = 5000;
constexpr int HEADER_SIZE = 4;
constexpr int FRAME_OVERHEAD = 5;

std::string buildFrame(const std::string& payload);
bool sendMsg(int fd, const std::string& data);
bool extractFrame(std::string& buf, std::string& out);
