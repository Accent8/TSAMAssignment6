#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <string>
 
#include <unistd.h>
#include <poll.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "protocol.h"

//connects to the server
int connecttoServer(const char* ip, const char* port){
    //describe what kind of address we want
    struct addrinfo hints, *svr; //hint= what you are asking for, svr(server) points to the results
    memset(&hints, 0, sizeof(hints));
    hints.ai_family= AF_INET; //ipv4
    hints.ai_socktype= SOCK_STREAM; //tcp

    int result= getaddrinfo(ip, port, &hints, &svr);
    if (result != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        return -1;
    }

    //getaddrinfo can ret many addresses, try all until conects
    int socket= -1;
    for(p= svr; p != nullptr; p->ai_next){
        socket= socket(p-> ai_family, p-> ai_socktype, p-> ai_protocol); //socket(svr->ai_family, svr->ai_socktype, svr->ai_protocol) ef thad a ad vera haegt ad breita hints
        if (socket < 0) continue;
        if (connect(socket, svr-> ai_addr, svr->ai_addrlen)== 0) break; //connected
        close(sock);
        socket= -1;
    }

   freeaddrinfo(svr);
   if (socket <0){
    perror("could not connect to server");
   }
   return socket;
}

//packs a comand line in a frame and sends to server
bool sendCommand(int sock, std::string line){
    //if (!line.empty() && line.back()== '\r') line.pop_back(); //Remove a trailing '\r' (e.g. input pasted from Windows)
    if (line.empty()) return true;

    std::string frame= packMsg(line);
    if (frame.empty()){
        printf("command too long, not sent\n");
        return true;
    }

    if (!sendMsg(sock, frame)){
        perror("failed sending to server");
        return false;
    }

    printf("[%s] sent: %s\n", timestamp().c_str(), line.c_str()); //std::cout << "[" << timestamp() << "] SENT: " << line << std::endl;
    fflush(stdout); //makes sure the line is written out right away
    return true;

}

bool userInput(int sock, std::string& inputBuffer){
    char buffer[1024];
    ssize_t bytes_read= read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytes_read<0){
        if(errno== EINTR) return true;
        perror("reading from stdin failed");
        return false;
    }
    if(bytes_read== 0){
        if (!inputBuffer.empty()) sendCommand(sock, inputBuffer);
        return false;
    }
    inputBuffer.append(buffer, bytes_read);

    size_t pos;
    while (true)
    {
        size_t pos= inputBuffer.find('\n');
        if(pos== std::string::npos){
            break; //no complete line ready. wait for more
        }
        std::string line= inputBuffer.substr(0, pos);
        inputBuffer.erase(0, pos + 1);
        if (!sendCommand(sock, line)) return false;
    }
    return true;
    
}

int main(int argc, char* argv[]) {

    
    int socket;
    //int nread;
    //char buffer[1025];
    //bool finished;

    //check that we got 2 arguments
    if(argc != 3){
        printf("should be 3 arguments: <client> <ip> <port>");
        printf("ctrl-c to terminate");
        exit(1);
    }

    //describe what kind of address we want


    //turn the ip and port strings into a socket address we can connect to


   //blabla

   close(socket);
   return 0;


    
}
