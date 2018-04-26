#include <opencog/guile/SchemePrimitive.h>
#include "crossguid/guid.hpp"
#include "RpcSyncExecutor.h"
#include "WebSocketIO/Server.h"

using namespace opencog;
using namespace std;

RpcSyncExecutor::RpcSyncExecutor(Socket *socket)
{
    _socket = socket;

    //todocd 
    //result to map
    socket->on("rpc-result!", [](JSON data) {
        cout << "receive rpc-result!: " << data.dump() << endl;
        string guid = data.at("guid").get<std::string>();
        string result = data.at("result").get<std::string>();
        cout << "receive rpc-result!: " << result << endl;
        //map[guid] = result;
    });
}

RpcSyncExecutor::~RpcSyncExecutor()
{
    logger().debug("[RpcSyncExecutor] destructor");
}

const string &RpcSyncExecutor::call(const string &method, const string &params)
{

    cout << "rpc-execute! hello" << endl;
    //{"method":"method_name","params":[xx,123,xxx]}

    //todo
    //add uuid
    //add to map

    string guid = xg::newGuid().str();
    //_result_map[guid] = NULL;
    // socket->send("rpc-execute!",
    //              "{\"method\":" + method +
    //                  "\"guid\":" + guid +
    //                  ",\"params\":[" + params + "]}");

    map<string, string>::iterator it;
    //string result;
    while (true)
    {
        it = _result_map.find(guid);
        if (it != _result_map.end())
        {
            return it->second;
        }
    }

}