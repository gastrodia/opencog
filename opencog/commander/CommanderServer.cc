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

#include <stdio.h>
#include <pthread.h>

#include <opencog/guile/SchemePrimitive.h>
#include "WebSocketIO/Server.h"
#include "CommanderServer.h"
#include "RpcSyncExecutor.h"
#include <exception>
#include "sole/sole.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace opencog;
using namespace std;

using json = nlohmann::json;

CommanderServer::CommanderServer()
{
}

void CommanderServer::init()
{
}

int CommanderServer::start_server()
{

    auto server = new Server("web");

    cout << "CommanderServer::start_server main thread_id: " << pthread_self() << endl;

    server->onConnection([server](Socket *socket) {

        cout << "CommanderServer::start_server on connection thread_id: " << pthread_self() << endl;

        //auto rpc = new RpcSyncExecutor(socket);

        socket->on("rpc-execute!", [server, socket](JSON data) {
            //TODO
            socket->send("rpc-result!", data.dump() + " world!");
        });

        try
        {
            cout << "try call client foo(\"hello\")" << endl;
            string guid = sole::uuid0().str();

            string result;

            socket->on("rpc-result!", [](JSON data) {
                cout << "RpcSyncExecutor::RpcSyncExecutor receive rpc-result! thread_id: " << pthread_self() << endl;
                cout << "receive rpc-result!: " << data.dump() << endl;
                string guid = data.at("guid").get<std::string>();
                cout << "receive guid!: " << guid << endl;
                string result = data.at("result").get<std::string>();
                cout << "receive result: " << result << endl;
                //update_map(guid, result);
                //update_as(guid, result);
                ofstream out(string("/tmp/commander_") + guid);
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
            });

            std::thread t = std::thread([&result, &socket, &guid]() {
                std::cout << "call client foo(\"hello\") std::thread thread_id: " << pthread_self() << std::endl;
                //result = rpc->call("foo", "[\"hello\"]");
                //rpc->call("foo", "[\"hello\"]");

                //{"method":"method_name","params":[xx,123,xxx]}
                string method = "foo";
                string params = "[\"hello\"]";
                string call_msg_str = std::string("") +
                                      "{\"method\":\"" + method + "\"" +
                                      ",\"guid\":" + "\"" + guid + "\"" +
                                      ",\"params\":" + params + "}";
                cout << "call_msg_str: " << call_msg_str << endl;
                socket->send("rpc-execute!", json::parse(call_msg_str));
                cout << "send msg success" << endl;
            });

            while (true)
            {
                sleep(1);
                //如果这里能调一次mongoose的event loop是最好的
                server->poll();
                ifstream in(string("/tmp/commander_") + guid);
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

            std::cout << "call client foo(\"hello\") result: " << result << std::endl;
            std::cout << "call client foo(\"hello\") get result thread_id: " << pthread_self() << std::endl;

            //cout << "mock call" << endl;
        }
        catch (std::exception &e)
        {
            cout << "got error" << endl;
            std::cerr << e.what() << endl;
            cout << e.what() << endl;
        }
        cout << "finish call! " << endl;

        socket->on("disconnect", [server, socket](JSON) {
            //server->broadcast("leave", {{"addr", socket->addr()}});
        });

        socket->on("close-server", [server](JSON) {
            server->close();
        });

    });

    std::thread t = std::thread([&server]() {
        std::cout << "main std::thread thread_id: " << pthread_self() << std::endl;
        server->listen(8000, 200);
    });
    while (true)
    {
        sleep(1);
    }

    return 0;
}
