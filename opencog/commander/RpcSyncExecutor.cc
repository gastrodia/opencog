
#include "sole/sole.hpp"
#include "RpcSyncExecutor.h"
#include <iostream>  
#include <fstream>  
#include <iomanip>  

using namespace opencog;
using namespace std;

// for convenience

RpcSyncExecutor::RpcSyncExecutor(Socket *socket)
{

    _socket = socket;

    map<string, string> result_map = {};
    result_map["abc"] = "cba";
    cout << "result_map: " << &result_map << endl;

    cout << "test atomspace" << endl;

    AtomSpace *as = new AtomSpace();
    SchemeEval *scm = new SchemeEval(as);
    cout << "(add-to-load-path \"/usr/local/share/opencog/scm\")" << endl;
    scm->eval("(add-to-load-path \"/usr/local/share/opencog/scm\")");

    // Load required modules for testing and populate the atomspace
    cout << "(use-modules (opencog))" << endl;
    scm->eval("(use-modules (opencog))");
    // cout << "(ConceptNode \"abc\")" << endl;
    // scm->eval("(ConceptNode \"abc\")");
    // cout << "(prt-atomspace)" << endl;
    // cout << as->to_string() << endl;
    // cout << "finish test" << endl;

    string simple_result;

    auto update_map = [&result_map](string &guid, string &result) {

        cout << "result_map: " << &result_map << endl;

        result_map[guid] = result;
        cout << "update result_map success! " << endl;
    };

    auto update_as = [&as, &scm, &simple_result](string &guid, string &result) {
        //scm->eval(std::string("") + "(ConceptNode \"" + guid + "\")");
        //cout << as->to_string() << endl;

        simple_result = result;
        cout << "update _as success! " << endl;

    };

    auto update_record_file = [](string &guid, string &result) {
        ofstream out(string("/tmp/commander-") + guid);
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

const string &RpcSyncExecutor::call(const string &method, const string &params)
{

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
        _socket->send("rpc-execute!", call_msg_str);
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
    cout << "send msg success" << endl;
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