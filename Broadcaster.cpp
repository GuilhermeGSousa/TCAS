#include "Broadcaster.hpp"

Broadcaster::Broadcaster(int portNum){

    setReceiver(portNum);
    setSender(portNum,"127.0.0.1");

}

void Broadcaster::setSender(int portNum, const char* dest_addr){
    send_sock = socket(AF_INET, SOCK_DGRAM, 0);

    sendAddr.sin_family = AF_INET;
    sendAddr.sin_port = htons(portNum);
    sendAddr.sin_addr.s_addr = inet_addr(dest_addr); //Mudar
    memset(sendAddr.sin_zero, '\0', sizeof sendAddr.sin_zero); 

}

void Broadcaster::setReceiver(int portNum){
    rcv_sock = socket(AF_INET, SOCK_DGRAM, 0);

    rcvAddr.sin_family = AF_INET;
    rcvAddr.sin_port = htons(portNum);
    rcvAddr.sin_addr.s_addr = INADDR_ANY;
    memset(rcvAddr.sin_zero, '\0', sizeof rcvAddr.sin_zero);

    bind(rcv_sock, (struct sockaddr*)&rcvAddr,sizeof(rcvAddr));
}

int Broadcaster::sendBuffer(char* buffer){
    int nBytes;
    nBytes=sendto(send_sock,buffer,strlen(buffer)+1,0,(struct sockaddr*)&sendAddr,sizeof(sendAddr));
    return nBytes;
}

int Broadcaster::receiveBuffer(char* buffer){
    int nBytes;
    nBytes = recvfrom(rcv_sock,buffer,1024,0, (struct sockaddr *)&serverStorage, &server_size);
    return nBytes;
}

Broadcaster::~Broadcaster(){

}
