#include <opencog/guile/SchemePrimitive.h>
#include "RpcSyncExecutor.h"

using namespace opencog;
using namespace std;

RpcSyncExecutor::RpcSyncExecutor(Socket *socket)
{
    _socket = socket;
    //todo 
    //result to map
    socket->on("rpc-result!", [server, socket](JSON data) {
        cout << "receive rpc-result!: " << data.dump() << endl;
        cout << "receive rpc-result!: " << data.at("result").get<std::string>() << endl;
    });
}

RpcSyncExecutor::~RpcSyncExecutor()
{
    logger().debug("[RpcSyncExecutor] destructor");
}

std::string &RpcSyncExecutor::call(std
                                   : string &method, std
                                   : string &params)
{

    cout << "rpc-execute! hello" << endl;
    //{"method":"method_name","params":[xx,123,xxx]}

    //todo
    //add uuid
    //add to map
    socket->send("rpc-execute!", "{\"method\":" + method + ",\"params\":[" + params + "]}");

    while(true){
        //sleep 0.1
        //get data from map 
        //return 
    }
}