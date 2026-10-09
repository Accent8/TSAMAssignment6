#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cerrno>
#include <poll.h>
#include <vector>
#include <algorithm>
#include <map>

int setUpListen(int port) {
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    if (setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("sockopt failed");
        close(listenSock);
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // accept connections on any local ip
    addr.sin_port = htons(port);

    if (bind(listenSock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(listenSock);
        return -1;
    }

    if (listen(listenSock, 10) < 0) { // 10 = backlog of pending connections
        perror("listen");
        close(listenSock);
        return -1;
    }
    return listenSock;
}

void acceptClient(int listenSock, std::vector<pollfd>& fds) {
    int client = accept(listenSock, nullptr, nullptr);
    if (client < 0) {
        perror("accept");
        return;
    }
    fds.push_back({client, POLLIN, 0});
    std::cout << "Client " << client << " connected" << std::endl;
    return;
}

bool handleClient(int clientSock, std::string& clientBuff) {
    char buff[2048];
    ssize_t n = recv(clientSock, buff, sizeof(buff), 0);
    size_t pos;
    clientBuff.append(buff, n);
    if (n > 0) {
        while((pos = clientBuff.find('\n')) != std::string::npos) {
            std::string msg = clientBuff.substr(0, pos);
            clientBuff.erase(0, pos + 1);
            std::cout << "Client " << clientSock << ": " << msg << std::endl;
        }
        return true;
    } else if (n == 0) {
        std::cout << "Client " << clientSock << " disconnected" << std::endl;
        return false;
    } else {
        if (errno == EINTR) {
            return true;
        } else {
            perror("recv");
            return false;
        }
    }
}

void runServer(int listenSock) {
    std::vector<pollfd> fds;
    std::map<int, std::string> clientBuffers;
    fds.push_back({listenSock, POLLIN, 0}); // index 0 - listener

    while (true) {
        int ready = poll(fds.data(), fds.size(), -1); // block until something happens
        if (ready < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("poll");
                break;
            }
        }

        for(size_t i = 0; i < fds.size(); i++) {
            if (!(fds[i].revents & POLLIN)) continue; // nothing happens here

            if (fds[i].fd == listenSock) {
                acceptClient(listenSock, fds);
            } else {
                bool clientStatus = handleClient(fds[i].fd, clientBuffers[fds[i].fd]);
                if (clientStatus == false) {
                    close(fds[i].fd);
                    clientBuffers.erase(fds[i].fd);
                    fds[i].fd = -1;
                }
            }
        }
        auto newFds = std::remove_if(fds.begin(), fds.end(), [](const pollfd& p) {return p.fd == -1;});
        fds.erase(newFds, fds.end());
    }

}

int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "incorrect amount of args" << std::endl;
        return 1;
    }

    int listenFD = setUpListen(std::stoi(argv[1]));
    if (listenFD < 0) {
        std::cerr << "listenSocket failed!" << std::endl;
        return 1;
    }
    runServer(listenFD);

    return 0;
}

