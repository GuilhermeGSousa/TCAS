#include "broadcaster.h"
#include <QDebug>
Broadcaster::Broadcaster(int portNum){

    setReceiver(portNum);
    setSender(portNum,"255.255.255.255");

}

uint32_t Broadcaster::checksumCalc(char* message){

        boost::crc_32_type crc;

        crc.process_bytes(message,BUFFSIZE-4);

        return crc.checksum();
}

void Broadcaster::setSender(int portNum, const char* dest_addr){
    send_sock = socket(AF_INET, SOCK_DGRAM, 0);

    sendAddr.sin_family = AF_INET;
    sendAddr.sin_port = htons(portNum);
    sendAddr.sin_addr.s_addr = inet_addr(dest_addr); //mudar
    //memset(sendAddr.sin_zero, '\0', sizeof sendAddr.sin_zero);
    int perm = 1;
    if(setsockopt(send_sock,SOL_SOCKET,SO_BROADCAST,(void *)&perm,sizeof(perm))<0){
        qDebug()<<"Permisson Error";
    }
}

void Broadcaster::setReceiver(int portNum){
    rcv_sock = socket(AF_INET, SOCK_DGRAM, 0);

    rcvAddr.sin_family = AF_INET;
    rcvAddr.sin_port = htons(portNum);
    rcvAddr.sin_addr.s_addr = INADDR_ANY;
    memset(rcvAddr.sin_zero, '\0', sizeof rcvAddr.sin_zero);

    bind(rcv_sock, (struct sockaddr*)&rcvAddr,sizeof(rcvAddr));
}

void Broadcaster::doubleToBuff(char *out,double in){
    memcpy(out,(double *)&in,sizeof(double));
}

double Broadcaster::bufferToDouble(char * buffer){
    double out;

    memcpy((double *)&out,buffer,sizeof(double));
    return out;
}

void Broadcaster::uint64ToBuff(char *out,uint64_t in){

    memcpy(out,(uint64_t *)&in,sizeof(uint64_t));
}

void Broadcaster::uint32ToBuff(char *out, uint32_t in){

   memcpy(out,(uint32_t *)&in,sizeof(uint32_t));
}


void Broadcaster::messageToBuffer(char* buffer, Message out){

    int index=0;
    char aux8[8],aux4[4];
    char buff[BUFFSIZE];

    for(int i=0;i<16;++i){
        buff[i+index]=out.header[i];
    }
    index+=16;


    //AC id
    uint64ToBuff(aux8,out.Ac_id);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;


    //Pos and Speed
    doubleToBuff(aux8,out.X_pos);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    doubleToBuff(aux8,out.Y_pos);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    doubleToBuff(aux8,out.Z_pos);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    doubleToBuff(aux8,out.X_spd);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    doubleToBuff(aux8,out.Y_spd);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    doubleToBuff(aux8,out.Z_spd);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    //TCAS status
    for(int i=0;i<16;++i){
        buff[i+index]=out.TCAS_status[i];
    }
    index+=16;

    //Intruder hex
    uint64ToBuff(aux8,out.Intruder_hex);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    //Resolution
    for(int i=0;i<16;++i){
        buff[i+index]=out.Resolution[i];
    }
    index+=16;

    //Resolution value
    doubleToBuff(aux8,out.Resolution_val);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    //Checksuout

    uint32ToBuff(aux4,out.CRC_32);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }


    for (int i = 0; i < BUFFSIZE; ++i)
    {
        buffer[i]=buff[i];
    }

}

Message Broadcaster::bufferToMessage(char buffer[BUFFSIZE]){
    Message out;
    int index = 0;
    char aux[8];
    for (int i = 0; i < 16; ++i)
    {
        out.header[i]=buffer[i+index];
    }index+=16;

    //Corrigir!

    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }
    memcpy((uint64_t*)&out.Ac_id,aux,sizeof(uint64_t));
    //out.Ac_id = ((unsigned long)buffer[0+index] << 56) + ((unsigned long)buffer[1+index] << 48) + ((unsigned long)buffer[2+index] << 40) + ((unsigned long)buffer[3+index] << 32) +
    //            ((unsigned long)buffer[4+index] << 24) + ((unsigned long)buffer[5+index] << 16) + ((unsigned long)buffer[6+index] << 8) + (unsigned long)buffer[7+index];

    index+=8;

    //Speed and Pos
    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }index+=8;
    out.X_pos = bufferToDouble(aux);

    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }index+=8;
    out.Y_pos = bufferToDouble(aux);

    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }index+=8;
    out.Z_pos = bufferToDouble(aux);

    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }index+=8;
    out.X_spd = bufferToDouble(aux);

    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }index+=8;
    out.Y_spd = bufferToDouble(aux);

    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }index+=8;
    out.Z_spd = bufferToDouble(aux);

    //TCAS status
    for (int i = 0; i < 16; ++i)
    {
        out.TCAS_status[i]=buffer[i+index];
    }index+=16;

    //Intruder
    memcpy((uint64_t*)&out.Intruder_hex,aux,sizeof(uint64_t));
    index+=8;

    //Resolution
    for (int i = 0; i < 16; ++i)
    {
        out.Resolution[i]=buffer[i+index];
    }index+=16;

    for (int i = 0; i < 8; ++i)
    {
        aux[i]=buffer[i+index];
    }index+=8;
    out.Resolution_val = bufferToDouble(aux);

    //Checksum
    for (int i = 0; i < 4; ++i)
    {
        aux[i]=buffer[i+index];
    }
    memcpy((uint32_t*)&out.CRC_32,aux,sizeof(uint32_t));
    index+=4;

    return out;
}

int Broadcaster::sendBuffer(char* buffer){
    int nBytes;
    nBytes=sendto(send_sock,buffer,BUFFSIZE,0,(struct sockaddr*)&sendAddr,sizeof(sendAddr));
    return nBytes;
}

void Broadcaster::listenBuffer()
{
    char b[BUFFSIZE];
    while(true)
        receiveBuffer(b);
}

int Broadcaster::receiveBuffer(char* buffer){
    int nBytes;
    nBytes = recvfrom(rcv_sock,buffer,BUFFSIZE,0, (struct sockaddr *)&serverStorage, &server_size);
    emit messageReceived(buffer);
    return nBytes;
}

Broadcaster::~Broadcaster(){

}
