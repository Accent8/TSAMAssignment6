#pragma once

#include <string>
#include <cstddef>

constexpr char SOH = 0x01;
constexpr char STX = 0x02;
constexpr char ETX = 0x03;
constexpr size_t MAX_FRAME = 5000;
constexpr size_t HEADER_SIZE = 4;
constexpr size_t FRAME_OVERHEAD = 5;

// packs a payload to include the header given in the assignment
// returns an empty string if the payload is to large
std::string packMsg(const std::string& payload);
bool sendMsg(int fd, const std::string& data);
// Extracts one complete frame from the front of buff into out
// Returns false if more data is needed. malformed bytes are discarded
// if discarded, the number of discarded bytes is added to it
bool unpackMsg(std::string& buf, std::string& out, size_t* discarded = nullptr);
std::string timestamp();
