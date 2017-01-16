#include "Broadcaster.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
    Broadcaster broadcaster = Broadcaster(10505);
    char buffer[1024];
    while(1){
        broadcaster.receiveBuffer(buffer);
        cout<<buffer<<endl;
    }

    return 0;
}