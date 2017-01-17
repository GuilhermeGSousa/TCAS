#include "Broadcaster.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
    Broadcaster broadcaster = Broadcaster(7891);
    char buffer[BUFFSIZE];
    // while(1){
    //     broadcaster.receiveBuffer(buffer);
    //     cout<<buffer<<endl;
    //     cout<<"Received"<<endl;
    // }

    Message m;

    strcpy(m.header,"OLA!");
    m.Intruder_hex=65;
    strcpy(m.TCAS_status,"HEYO");


    broadcaster.messageToBuffer(buffer, m);



    cout<<(int)buffer[95]<<endl;;


    return 0;
}