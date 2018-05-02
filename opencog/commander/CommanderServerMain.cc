#include "CommanderServer.h"

using namespace opencog;
using namespace std;
int main(int argc, char *argv[])
{

    //参数变量初始化
    string port;
    string scm_file;

    //获取参数
    int ch;
    while ((ch = getopt(argc, argv, "p:s:")) != -1)
    {
        switch (ch)
        {
        case 'p':
            port = string(optarg);
            break;
        case 's':
            scm_file = string(optarg);
            break;
        }
    }

    cout << "port: " << port << endl;
    cout << "scm_file: " << scm_file << endl;

    auto server = new CommanderServer();
    server->port = std::stoi(port);
    server->scm_file = scm_file;
    server->start_server();
    return 0;
}
