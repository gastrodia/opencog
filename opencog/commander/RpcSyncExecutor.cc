#include <opencog/guile/SchemePrimitive.h>
#include "sole/sole.hpp"
#include "RpcSyncExecutor.h"

using namespace opencog;
using namespace std;

RpcSyncExecutor::RpcSyncExecutor(Socket *socket)
{
    map<string, string> result_map = {};
    
    _socket = socket;
    _result_map = result_map;
    _socket->on("rpc-result!", [&result_map](JSON data) {
        cout << "receive rpc-result!: " << data.dump() << endl;
        string guid = data.at("guid").get<std::string>();
        string result = data.at("result").get<std::string>();
        result_map[guid] = result;
    });
}

RpcSyncExecutor::~RpcSyncExecutor()
{
    logger().debug("[RpcSyncExecutor] destructor");
}

const string &RpcSyncExecutor::call(const string &method, const string &params)
{

    //{"method":"method_name","params":[xx,123,xxx]}
    string guid = sole::uuid0().str();
    _socket->send("rpc-execute!",
                 "{\"method\":" + method +
                     "\"guid\":" + guid +
                     ",\"params\":[" + params + "]}");

    map<string, string>::iterator it;
    //string result;
    while (true)
    {
        it = _result_map.find(guid);
        if (it != _result_map.end())
        {
            _result_map.erase(it);  
            return it->second;
        }
    }

}