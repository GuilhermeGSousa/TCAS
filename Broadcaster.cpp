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

void Broadcaster::doubleToBuff(char *out,double in){
    union{
        double doubleVal;
        char buff[sizeof(doubleVal)];
    }u;

    for (int i = 0; i < sizeof(u.doubleVal); ++i)
    {
        out[i]=u.buff[i];
    }
}
void Broadcaster::uint64ToBuff(char *out,uint64_t in){
    uint64_t aux=in;
    for(int i=0; i<8; i++){

        out[i]= (in & 0xFF);
        aux=(aux >> 8);
    }
}
void Broadcaster::uint32ToBuff(char *out, uint32_t in){
    uint32_t aux=in;
    for(int i=0; i<4; i++){

        out[i]= (in & 0xFF);
        aux=(aux >> 8);
    }
}
void Broadcaster::messageToBuffer(char* buffer, Message m){

    int index=0;
    char aux8[8],aux4[4];
    char buff[BUFFSIZE];

    for(int i=0;i<16;++i){
        buff[i+index]=m.header[i];
    }
    index+=16;


    //AC id
    uint64ToBuff(aux8,m.Ac_id);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;


    //Pos and Speed
    doubleToBuff(aux8,m.X_pos);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;
    
    doubleToBuff(aux8,m.Y_pos);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;
    
    doubleToBuff(aux8,m.Z_pos);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    doubleToBuff(aux8,m.X_spd);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;
    
    doubleToBuff(aux8,m.Y_spd);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;
    
    doubleToBuff(aux8,m.Z_spd);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;
    
    //TCAS status
    for(int i=0;i<16;++i){
        buff[i+index]=m.TCAS_status[i];
    }
    index+=16;

    //Intruder hex
    uint64ToBuff(aux8,m.Intruder_hex);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;
    cout<<index<<endl;
    //Resolution
    for(int i=0;i<16;++i){
        buff[i+index]=m.Resolution[i];
    }
    index+=16;

    //Resolution value
    doubleToBuff(aux8,m.Resolution_val);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=8;

    //??????

    uint32ToBuff(aux4,m.CRC_32);
    for (int i = 0; i < 8; ++i)
    {
        buff[i+index]=aux8[i];
    }
    index+=4;

    for (int i = 0; i < BUFFSIZE; ++i)
    {
        buffer[i]=buff[i];
    }

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
