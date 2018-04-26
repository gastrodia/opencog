
#include "sole/sole.hpp"
#include "RpcSyncExecutor.h"
#include "WebSocketIO/json/json.hpp"

using namespace opencog;
using namespace std;

// for convenience
using json = nlohmann::json;

RpcSyncExecutor::RpcSyncExecutor(Socket *socket)
{
    const map<string, string> result_map = {};

    cout << "result_map: " << &result_map << endl;

    cout << "test atomspace" << endl;
    _as = new AtomSpace();
    _scm = new SchemeEval(_as);
    cout << "(add-to-load-path \"/usr/local/share/opencog/scm\")" << endl;
    _scm->eval("(add-to-load-path \"/usr/local/share/opencog/scm\")");

    // Load required modules for testing and populate the atomspace
    cout << "(use-modules (opencog))" << endl;
    _scm->eval("(use-modules (opencog))");
    cout << "(define (prt-atomspace)\
        (define (prt-atom h)\
            ; print only the top-level atoms.\
            (if (null? (cog-incoming-set h))\
                (display h))\
        #f)\
        (define (prt-type type)\
            (cog-map-type prt-atom type)\
            ; We have to recurse over sub-types\
            (for-each prt-type (cog-get-subtypes type))\
        )\
        (prt-type 'Atom)\
        )"
         << endl;
    _scm->eval("(define (prt-atomspace)\
        (define (prt-atom h)\
            ; print only the top-level atoms.\
            (if (null? (cog-incoming-set h))\
                (display h))\
        #f)\
        (define (prt-type type)\
            (cog-map-type prt-atom type)\
            ; We have to recurse over sub-types\
            (for-each prt-type (cog-get-subtypes type))\
        )\
        (prt-type 'Atom)\
        )");
    cout << "(ConceptNode \"abc\")" << endl;
    _scm->eval("(ConceptNode \"abc\")");
    cout << "(prt-atomspace)" << endl;
    cout << _scm->eval("(prt-atomspace)") << endl;
    cout << "finish test" << endl;

    // auto update_map = [](string &guid, string &result) {

    //     //cout << "result_map: " << &result_map << endl;
    //     //_scm->eval(std::string("") + "(ConceptNode \"" + guid + "\")");
    //     //get_all_atoms

    //     //result_map.insert(std::pair<string, string>("abc", "cba"));
    //     //result_map["abc"] = "cba";
    //     //cout << result_map.at("abc") << endl;

    //     //result_map[guid] = result;

    //     cout << "update result_map success! " << endl;
    // };

    // auto on_result = [&update_map](JSON data) {
    //     cout << "receive rpc-result!: " << data.dump() << endl;
    //     string guid = data.at("guid").get<std::string>();
    //     cout << "receive guid!: " << guid << endl;
    //     string result = data.at("result").get<std::string>();
    //     cout << "receive result: " << result << endl;
    //     update_map(guid, result);
    //     //count << result_map.at(guid) << endl;
    // };

    // auto data = json::parse(std::string("") +
    //                         "{\"method\":" + "\"foo\"" +
    //                         ",\"guid\":" + "\"" + "abc" + "\"" +
    //                         ",\"result\":" + "\"" + "hello world" + "\"" +
    //                         ",\"params\":" + "[\"hello\"]" + "}");
    // //on_result(data);
    // _socket->on("rpc-result!", on_result);
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