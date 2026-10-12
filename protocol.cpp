#include <iostream>
#include "protocol.h"
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>

std::string packMsg(const std::string& payload) {
    std::string frame;
    size_t totalLength = payload.size() + FRAME_OVERHEAD;
    if (totalLength > MAX_FRAME) {
        return frame;
    }
    uint16_t netLen = htons(static_cast<uint16_t>(totalLength));

    frame += SOH;
    frame.append(reinterpret_cast<const char*>(&netLen), 2);
    frame += STX;
    frame += payload;
    frame += ETX;
    return frame;
}

bool sendMsg(int fd, const std::string& data) {
    size_t sent = 0;
    while (sent < data.size()) {
        ssize_t n = send(fd, (data.data() + sent), (data.size() - sent), 0);
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                return false;
            }
        } else {
            sent += n;
        }
    }
    return true;
}

bool unpackMsg(std::string& buf, std::string& out) {
    while (true) {
        if (buf.length() < HEADER_SIZE) {
            return false;
        }
        if (buf[0] != SOH) {
            size_t pos = buf.find(SOH);
            if (pos != std::string::npos) {
                std::cerr << "Skipping til next SOH" << std::endl;
                buf.erase(0, pos);
                continue;
            } else {
                buf.clear();
                std::cerr << "No SOH" << std::endl;
                return false;
            }
        }
        uint16_t netLen;
        std::memcpy(&netLen, buf.data() + 1, 2);
        uint16_t len = ntohs(netLen);
        if (len < FRAME_OVERHEAD || len > MAX_FRAME || buf[3] != STX) {
            std::cerr << "Bad STX or length out of range" << std::endl;
            buf.erase(0, 1); // Drop the first byte
            continue;
        }
        if (buf.size() < len) {
            return false;
        }
        if (buf[len - 1] != ETX) {
            std::cerr << "Bad ETX" << std::endl;
            buf.erase(0, 1);
            continue;
        }
        out = buf.substr(HEADER_SIZE, len - FRAME_OVERHEAD);
        buf.erase(0, len);
        return true;
    }
}

std::string timestamp(){
    time_t curr= time(nullptr);
    char buf[32];
    strftime(buf, sizeof buf, "%d.%m.%Y - %H:%M:%S", localtime(&curr));
    return buf;
}