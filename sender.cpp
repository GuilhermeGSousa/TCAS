#include "Broadcaster.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
    Broadcaster broadcaster = Broadcaster(7891);
    char b[BUFFSIZE];

    Message m;
    uint64_t a = 65;
    strcpy(m.Resolution,"ola");
    m.Ac_id = a;
    m.X_pos=1.25;

    broadcaster.messageToBuffer(b,m);

    broadcaster.sendBuffer(b);

    return 0;
}