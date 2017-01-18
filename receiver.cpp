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
    m.X_pos=1.25;

    broadcaster.messageToBuffer(b,m);


    m=broadcaster.bufferToMessage(b);

    cout<<m.Resolution<<endl;
    cout<<m.Ac_id<<endl;
    cout<<m.X_pos<<endl;


    return 0;
}