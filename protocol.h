#pragma once

#include <string>
#include <cstddef>

constexpr char SOH = 0x01;
constexpr char STX = 0x02;
constexpr char ETX = 0x03;
constexpr size_t MAX_FRAME = 5000;
constexpr size_t HEADER_SIZE = 4;
constexpr size_t FRAME_OVERHEAD = 5;

std::string packMsg(const std::string& payload);
bool sendMsg(int fd, const std::string& data);
bool unpackMsg(std::string& buf, std::string& out);
std::string timestamp();