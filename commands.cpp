#include "commands.h"

static std::string handleStatusReq(const Mailbox& mailbox) {
    if (mailbox.empty()) {
        return "EMPTY";
    }
    std::string result;
    for (const auto& [user, queue] : mailbox) {
        if (!queue.empty()) {
            if (result.empty()) {
                result += user + ":" + std::to_string(queue.size());
            } else {
                result += "," + user + ":" + std::to_string(queue.size());
            }
        }
    }
    return result;
}

static std::string handleSendMsg(const std::string& args, Mailbox& mailbox) {
    // find the first comma: author of the message
    size_t c1 = args.find(',');
    if (c1 == std::string::npos) {
        return "ERROR, missing fields";
    }
    // find the second comma: recipient of the message
    size_t c2 = args.find(',', c1 + 1);
    if (c2 == std::string::npos) {
        return "ERROR, missing field";
    }

    // cut the message up
    std::string sender = args.substr(0, c1); // from the start to first comma
    std::string receiver = args.substr(c1 + 1, c2 - c1 - 1); // between the commas
    std::string text = args.substr(c2 + 1); // everything else

    // validate
    if (sender.empty() || receiver.empty()) {
        return "ERROR, empty field";
    }

    // store message if complete
    mailbox[receiver].push_back({sender, text});
    return "OK";
}

static std::string handleGetMsg(const std::string& user, Mailbox& mailbox) {
    if (user.empty()) {
        return "ERROR, missing user";
    }

    auto entry = mailbox.find(user);
    if (entry == mailbox.end()) {
        return "NONE";
    }
    std::deque<Message>& msgQueue = entry->second;
    Message m = msgQueue.front();
    msgQueue.pop_front();

    if(msgQueue.empty()) {
        mailbox.erase(entry);
    }

    return m.from + "," + m.text;
}

std::string handleCommand(const std::string& cmd, Mailbox& mailbox) {
    size_t comma = cmd.find(',');
    std::string name = cmd.substr(0, comma);
    std::string args = (comma == std::string::npos) ? "" : cmd.substr(comma + 1);

    if (name == "STATUSREQ") return handleStatusReq(mailbox);
    else if (name == "SENDMSG") return handleSendMsg(args, mailbox);
    else if (name == "GETMSG") return handleGetMsg(args, mailbox); 
    else return "ERROR, unkown command";
}
