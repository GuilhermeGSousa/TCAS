#ifndef BROADCASTER_HPP
#define BROADCASTER_HPP



#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

#define BUFFSIZE 124

class Broadcaster
{
public:
    Broadcaster(int);
    int sendBuffer(char *);
    int receiveBuffer(char *);
    ~Broadcaster();
private:
    int send_sock, rcv_sock;
    socklen_t server_size;
    struct sockaddr_in sendAddr, rcvAddr, serverStorage;
    void setReceiver(int);
    void setSender(int,const char*);
};

#endif