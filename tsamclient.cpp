#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <stdio.h>
//#include <errno.h>
//#include <stdlib.h>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netinet/ip.h>
//#include <netinet/tcp.h>
#include <netdb.h>
//#include <arpa/inet.h>
//#include <string.h>
//#include <algorithm>
//#include <map>
//#include <vector>
#include <poll.h>

//#include <iostream>
//#include <sstream>
//#include <map>


int main(int argc, char* argv[]) {

    
    int serverSocket;
    //int nwrite; //maybe need, a local variable in sendAll() can replace it
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
    struct addrinfo hints, *svr; //hint= what you are asking for, svr(server) points to the results
    memset(&hints, 0, sizeof(hints));
    hints.ai_family= AF_INET; //ipv4
    hints.ai_socktype= SOCK_STREAM; //tcp

    //turn the ip and port strings into a socket address we can connect to
    int result= getaddrinfo(argv[1], argv[2], &hints, &svr);
    if (result != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        exit(1); // eda exit(EXIT_FAILURE) skoda betur https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    }
    /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully connect(2).
              If socket(2) (or connect(2)) fails, we (close the socket
              and) try the next address.  */

    //creates a tcp socket to talk to the server 
    serverSocket= socket(AF_INET, SOCK_STREAM, 0); //socket(svr->ai_family, svr->ai_socktype, svr->ai_protocol) ef thad a ad vera haegt ad breita hints
    if (serverSocket < 0){
        perror("failed to create socket");
        freeaddrinfo(svr);
        exit(1);
    }

    //connects to the server
    if (connect(serverSocket, svr-> ai_addr, svr->ai_addrlen) < 0){
        perror("failed to connect");
        //close(serverSocket); held ad thetta tharf ekki en gott
        freeaddrinfo(svr);
        exit(1);
    }
    //dont need address info after connecting
   freeaddrinfo(svr);

   //blabla

   close(serverSocket);
   return 0;


    
}
