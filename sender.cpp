#include "Broadcaster.hpp"
using namespace std;
int main(int argc, char const *argv[])
{
    Broadcaster broadcaster = Broadcaster(7891);
    char buffer[1024];
    while(1){
        printf("Type a sentence to send to server:\n");
        fgets(buffer,1024,stdin);   
        broadcaster.sendBuffer(buffer);
    }

    return 0;
}