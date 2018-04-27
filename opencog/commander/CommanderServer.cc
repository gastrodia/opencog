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

using json = nlohmann::json;
using namespace opencog;
using namespace std;

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

        socket->on("invoke-rpc-execute!", [server, socket](JSON data) {
            //for unit test
            auto rpc = new RpcSyncExecutor(server, socket);
            cout << "invoke-rpc-execute! " << data.dump() << endl;
            string method = data.at("method").get<std::string>();
            string guid = data.at("guid").get<std::string>();
            //string params = data.at("params").get<std::string>();
            string params = "[\"hello\"]";
            //cout << "invoke-rpc-execute! " << data.at("params").get<std::array>() << endl;
            string result =  rpc->call(method, params);
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

        socket->on("disconnect", [server, socket](JSON) {
            //server->broadcast("leave", {{"addr", socket->addr()}});
        });

        socket->on("close-server", [server](JSON) {
            server->close();
        });

    });

    server->listen(8000, 200);

    return 0;
}
