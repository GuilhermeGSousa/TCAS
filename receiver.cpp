#include "Broadcaster.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
    Broadcaster broadcaster = Broadcaster(7891);
    char b[BUFFSIZE];

    
    broadcaster.receiveBuffer(b);


    
    Message m=broadcaster.bufferToMessage(b);

    cout<<m.Resolution<<endl;
    cout<<m.Ac_id<<endl;
    cout<<m.X_pos<<endl;


    return 0;
}