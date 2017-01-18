#include "Broadcaster.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
    Broadcaster broadcaster = Broadcaster(7891);
    char b[BUFFSIZE];
    // while(1){
    //     broadcaster.receiveBuffer(buffer);
    //     cout<<buffer<<endl;
    //     cout<<"Received"<<endl;
    // }

    

    Message m;
    uint64_t a = 65;
    strcpy(m.Resolution,"ola");
    m.Ac_id = a;


    broadcaster.messageToBuffer(b,m);


    m=broadcaster.bufferToMessage(b);

    cout<<m.Resolution<<endl;
    cout<<m.Ac_id<<endl;
    return 0;
}