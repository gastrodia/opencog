#include <opencog/commander/CommanderServer.h>
int main(int argc, char *argv[])
{
    
    auto server = new CommanderServer();

    return server->start_server();
}
