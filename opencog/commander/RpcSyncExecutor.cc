
#include "sole/sole.hpp"
#include "RpcSyncExecutor.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <opencog/commander/WebSocketIO/json/json.hpp>

using json = nlohmann::json;

using namespace opencog;
using namespace std;

// for convenience




RpcSyncExecutor::RpcSyncExecutor(Socket *socket)
{

    pthread_t thread_id = pthread_self();
    cout << "RpcSyncExecutor::RpcSyncExecutor thread_id: " << thread_id << endl;
    _socket = socket;
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
    logger().debug("[RpcSyncExecutor] destructor");
}

string RpcSyncExecutor::get_task_record_file(string &guid)
{
    return string("/tmp/commander_") + guid;
};

string &RpcSyncExecutor::call(const string &method, const string &params)
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

    try
    {
        // do stuff that may throw or fail
        _socket->send("rpc-execute!", json::parse(call_msg_str));
        cout << "send msg success" << endl;
    }
    catch (const std::runtime_error &re)
    {
        // speciffic handling for runtime_error
        std::cerr << "Runtime error: " << re.what() << std::endl;
    }
    catch (const std::exception &ex)
    {
        // speciffic handling for all exceptions extending std::exception, except
        // std::runtime_error which is handled explicitly
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
    catch (...)
    {
        // catch any other errors (that we have no information about)
        std::cerr << "Unknown failure occurred. Possible memory corruption" << std::endl;
    }
    //_socket->send("rpc-execute!", call_msg_str);
    cout << "wait.." << endl;

    string result;

    // while (true)
    // {
    //     cout << "wait.." << endl;
    //     sleep(1);
    //     ifstream in(get_task_record_file(guid));
    //     if (in.is_open())
    //     {
    //         getline(in, result);
    //         in.close();
    //         return result;
    //     }
    //     else
    //     {
    //         cout << "cant't read file! " << endl;
    //     }
    // }

    //std::mutex m;
    //std::unique_lock<std::mutex> lk(m);
    // while (true)
    // {
    //     cout << "wait! " << endl;
    //     //lk.unlock();           // 1 解锁互斥量
    //     pthread_t thread_id = pthread_self();
    //     cout << "thread_id: " << thread_id << endl;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 2 休眠1000ms
    //     //lk.lock();                                                   // 3 再锁互斥量
    // }
}