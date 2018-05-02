/*
 * CommanderServer.cc
 *
 * Copyright (C) 2017 MindCloud
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "WebSocketIO/Server.h"
#include "CommanderServer.h"
#include "RpcSyncExecutor.h"
#include <exception>
#include <opencog/commander/WebSocketIO/json/json.hpp>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/guile/SchemeEval.h>
#include <opencog/guile/SchemePrimitive.h>

using json = nlohmann::json;
using namespace opencog;
using namespace std;

CommanderServer::CommanderServer()
{
}

void CommanderServer::init()
{
}

string CommanderServer::test_atomspace_handler(const string &method, const string &params)
{
    cout << "test atomspace" << endl;
    // Handle hlist;
    // Type t = h->getType();
    // if (classserver().isA(t, NODE))
    // {
    //     NodePtr n = NodeCast(h);
    //     std::string name = n->getName();
    //     printf("Info: my_func instance %d received the node: %s\n",
    //            _id, name.c_str());
    //     hlist = _as->add_link(LIST_LINK, h);
    // }
    // else
    // {
    //     printf("Warning: my_func instance %d called with invalid handle\n", _id);
    // }
    // return hlist;

    cout << "method: " << method << endl;
    cout << "params: " << params << endl;
    return "hello world";
}

int CommanderServer::start_server()
{

    auto server = new Server("web");

    cout << "CommanderServer::start_server main thread_id: " << pthread_self() << endl;

    server->onConnection([server, this](Socket *socket) {

        cout << "CommanderServer::start_server on connection thread_id: " << pthread_self() << endl;

        socket->on("invoke-rpc-execute!", [server, socket, this](JSON data) {
            //for unit test
            auto rpc = new RpcSyncExecutor(server, socket);
            cout << "invoke-rpc-execute! " << data.dump() << endl;
            string method = data.at("method").get<std::string>();
            string guid = data.at("guid").get<std::string>();
            //string params = data.at("params").get<std::string>();
            string params = "[\"hello\"]";
            //cout << "invoke-rpc-execute! " << data.at("params").get<std::array>() << endl;
            string result = rpc->call(method, params);
            cout << "rpc-call result: " << result << endl;
            string str = std::string("") +
                         "{\"method\":\"" + method + "\"" +
                         ",\"guid\":" + "\"" + guid + "\"" +
                         ",\"result\":" + "\"" + result + "\"" +
                         ",\"params\":" + params + "}";
            JSON msg = json::parse(str);
            cout << "invoke-rpc-result! " << msg.dump() << endl;
            socket->send("invoke-rpc-result!", msg);
        });

        socket->on("psi-run!", [server, socket, this](JSON data) {
            // Need to access the atomspace to get it to initialize itself.
            AtomSpace *as = new AtomSpace();

            // Do this early, so that guile is initialized.
            SchemeEval *eval = new SchemeEval(as);

            cout << "(add-to-load-path \"/usr/local/share/opencog/scm\")" << endl;
            eval->eval("(add-to-load-path \"/usr/local/share/opencog/scm\")");

            // Load required modules for testing and populate the atomspace
            cout << "(use-modules (opencog))" << endl;
            eval->eval("(use-modules (opencog))");
            if (!this->scm_file.empty())
            {
                string load_file_code = "(load-from-path \"" + this->scm_file + "\")";
                cout << load_file_code << endl;
                eval->eval(load_file_code);
                //check
                // string check_code = "(test_psi_get_action_1)";
                // cout << "###########################################################################################" << endl;
                // cout << check_code << endl;
                // cout << eval->eval("(test_psi_get_action_1)") << endl;
            }

            RpcSyncExecutor *rpc = new RpcSyncExecutor(server, socket);

            define_scheme_primitive("rpc-call", &RpcSyncExecutor::call, rpc);
            //define_scheme_primitive("rpc-call", &CommanderServer::test_atomspace_handler, this);
            cout << "(define nnn (cog-new-node 'ConceptNode \"Hello World!\"))" << endl;
            eval->eval("(define nnn (cog-new-node 'ConceptNode \"Hello World!\"))");
            cout << "(rpc-call)" << endl;

            string rslt = eval->eval("(rpc-call \"foo\" \"[\\\"hello\\\"]\")");
            cout << "rpc-call result: " << rslt << endl;
            if (eval->eval_error())
            {
                printf("Error: failed evaluation\n");
                cout << eval->eval_error() << endl;
            }
        });

        socket->on("disconnect", [server, socket](JSON) {
            //server->broadcast("leave", {{"addr", socket->addr()}});
        });

        socket->on("close-server", [server](JSON) {
            server->close();
        });

    });

    if (port)
    {
        server->listen(port, 200);
    }
    else
    {
        server->listen(8000, 200);
    }

    return 0;
}
