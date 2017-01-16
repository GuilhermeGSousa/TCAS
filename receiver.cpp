#include "Broadcaster.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
    Broadcaster broadcaster = Broadcaster(7891);
    char buffer[1024];
    while(1){
        broadcaster.receiveBuffer(buffer);
        cout<<buffer<<endl;
        cout<<"Received"<<endl;
    }

    return 0;
}