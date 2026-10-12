#include "protocol.h"
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cerrno>

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
    if (buf.length() < 4) {
        return false;
    }
    if (buf[0] != '\x01') {
        size_t pos = buf.find('\x01');
        if (pos != std::string::npos) {
            buf.erase(0, pos);
            continue;
        } else {
            buf.clear();
            return false;
        }
    }
    uint16_t netLen;
    std::memcpy(&netLen, buf.data() + 1, 2);
    uint16_t len = ntohs(netLen);
    if (len < 5 || len > 5000 || buf[3] != '\x02') {
        buf.erase(0, 1); // Drop the first byte
        continue;
    }
    if (buf.size() < len) {
        return false;
    }
    if (buf[len - 1] != '\x03') {
        buf.erase(0, 1);
        continue;
    }
    std::string out = buf.substr(HEADER_SIZE, len - FRAME_OVERHEAD);
    clientBuff.erase(0, len);
    return true;
}

std::string timestamp(){
    time_t curr= time(nullptr);
    char buf[32];
    strftime(buf, sizeof buf, "%d.%m.%Y - %H:%M:%S", localtime(&curr));
    return buf
}