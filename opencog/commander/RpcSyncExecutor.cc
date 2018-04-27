
#include <opencog/commander/sole/sole.hpp>
#include "RpcSyncExecutor.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <opencog/commander/WebSocketIO/json/json.hpp>

using json = nlohmann::json;

using namespace opencog;
using namespace std;

// for convenience

RpcSyncExecutor::RpcSyncExecutor(Server *server, Socket *socket)
{

    _socket = socket;
    _server = server;
    cout << "init rpc success: " << &_socket << endl;

    pthread_t thread_id = pthread_self();
    cout << "RpcSyncExecutor::RpcSyncExecutor thread_id: " << thread_id << endl;

    auto update_record_file = [this](string &guid, string &result) {
        ofstream out(this->get_task_record_file(guid));
        if (out.is_open())
        {
            out << result;
            out.close();
            cout << "write to file success! " << endl;
        }
        else
        {
            cout << "cant't write file! " << endl;
        }
    };

    auto on_result = [&update_record_file](JSON data) {

        pthread_t thread_id = pthread_self();
        cout << "RpcSyncExecutor::RpcSyncExecutor receive rpc-result! thread_id: " << thread_id << endl;

        cout << "receive rpc-result!: " << data.dump() << endl;
        string guid = data.at("guid").get<std::string>();
        cout << "receive guid!: " << guid << endl;
        string result = data.at("result").get<std::string>();
        cout << "receive result: " << result << endl;
        //update_map(guid, result);
        //update_as(guid, result);
        update_record_file(guid, result);
    };

    _socket->on("rpc-result!", on_result);
}

RpcSyncExecutor::~RpcSyncExecutor()
{
    //logger().debug("[RpcSyncExecutor] destructor");
}

string RpcSyncExecutor::get_task_record_file(string &guid)
{
    return string("/tmp/commander_") + guid;
};

string RpcSyncExecutor::call(const string &method, const string &params)
{
    pthread_t thread_id = pthread_self();
    cout << "RpcSyncExecutor::call thread_id: " << thread_id << endl;

    //{"method":"method_name","params":[xx,123,xxx]}
    string guid = sole::uuid0().str();

    string call_msg_str = std::string("") +
                          "{\"method\":\"" + method + "\"" +
                          ",\"guid\":" + "\"" + guid + "\"" +
                          ",\"params\":" + params + "}";
    cout << "call_msg_str: " << call_msg_str << endl;
    JSON msg = json::parse(call_msg_str);
    cout << "msg: " << msg.dump() << endl;
    cout << "_socket: " << &_socket << endl;
    _socket->send("rpc-execute!", msg);
    cout << "send call_msg success! " << endl;
    string result;

    while (true)
    {
        _server->poll();
        sleep(1);
        ifstream in(get_task_record_file(guid));
        if (in.is_open())
        {
            getline(in, result);
            in.close();
            break;
        }
        else
        {
            cout << "cant't read file! " << endl;
        }
    }

    return result;
}