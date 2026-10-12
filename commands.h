#pragma once

#include <string>
#include <map>
#include <deque>

struct Message {
    std::string from;
    std::string text;
};

using Mailbox = std::map<std::string, std::deque<Message>>;
// Parses one command, then updates the mailbox,
// returns the response payload to send back
std::string handleCommand(const std::string& cmd, Mailbox& mailbox);
