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

    server->onConnection([server](Socket *socket) {

        socket->on("rpc-execute!", [server, socket](JSON data) {
            //TODO
            socket->send("rpc-result!", data.dump() + " world!");
        });

        auto rpc = new RpcSyncExecutor(socket);
        cout << "try call client foo(\"hello\")" << endl;

        try
        {
            rpc->call("foo", "[\"hello\"]");
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

    server->listen(8000, 200);

    return 0;
}
