#include <opencog/guile/SchemePrimitive.h>
#include "sole/sole.hpp"
#include "RpcSyncExecutor.h"

using namespace opencog;
using namespace std;

RpcSyncExecutor::RpcSyncExecutor(Socket *socket)
{
    map<string, string> result_map = {};

    cout << "result_map: " << &result_map << endl;

    _socket = socket;
    _result_map = result_map;

    

    _socket->on("rpc-result!", [&result_map](JSON data) {
        cout << "receive rpc-result!: " << data.dump() << endl;
        string guid = data.at("guid").get<std::string>();
        cout << "receive guid!: " << guid << endl;
        string result = data.at("result").get<std::string>();
        cout << "receive result: " << result << endl;

        cout << "result_map: " << &result_map << endl;
        result_map.insert(std::pair<string, string>("abc", "cba"));
        //result_map["abc"] = "cba";
        cout << result_map.at("abc") << endl;

        result_map[guid] = result;

        cout << "update result_map success! " << endl;
        //count << result_map.at(guid) << endl;
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
                  "{\"method\":\"" + method + "\"" +
                      ",\"guid\":" + "\"" + guid + "\"" +
                      ",\"params\":" + params + "}");

    map<string, string>::iterator it;
    string result = "";
    cout << "wait.." << endl;
    // while (true)
    // {
    //     cout << "wait.." << endl;
    //     it = _result_map.find(guid);
    //     if (it != _result_map.end())
    //     {
    //         _result_map.erase(it);
    //         return it->second;
    //     }
    // }
    return "";
}