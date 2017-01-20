#ifndef BROADCASTER_H
#define BROADCASTER_H


#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

#define BUFFSIZE 124
using namespace std;
typedef struct
{
    char header[16];//16 bytes
    uint64_t Ac_id; //64 bits
    double X_pos; //64 bits na mesma mas este tem um bit de sinal
    double Y_pos;
    double Z_pos;
    double X_spd;
    double Y_spd;
    double Z_spd;
    char TCAS_status[16];
    uint64_t Intruder_hex;//Não sei o que e isto
    char Resolution[16];
    double Resolution_val;
    uint32_t CRC_32; //Checksum

    //Se não me enganei nas contas o total da 128 bytes
}Message;


class Broadcaster
{
public:
    Broadcaster(int);
    int sendBuffer(char *);
    int receiveBuffer(char *);
    void messageToBuffer(char *, Message);
    Message bufferToMessage(char *);
    uint32_t checksumCalc(char*);
    ~Broadcaster();
private:
    int send_sock, rcv_sock;
    socklen_t server_size;
    struct sockaddr_in sendAddr, rcvAddr, serverStorage;
    void setReceiver(int);
    void setSender(int,const char*);
    void doubleToBuff(char *,double);
    double bufferToDouble(char *);
    void uint64ToBuff(char *,uint64_t);
    void uint32ToBuff(char *,uint32_t);


};

#endif
