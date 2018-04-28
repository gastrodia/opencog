#include "CommanderServer.h"
using namespace opencog;
int main(int argc, char *argv[])
{
    
    auto server = new CommanderServer();

    server->start_server();
    return  0;
}
