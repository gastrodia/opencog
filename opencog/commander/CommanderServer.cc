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

#include <opencog/guile/SchemePrimitive.h>
#include "CommanderServer.h"
#include "WebSocketIO/Server.h"


using namespace opencog;

CommanderServer::CommanderServer()
{
}

void CommanderServer::init()
{
}



int CommanderServer::start_server() 
{

    auto server = new Server("web");

    server->onConnection([server](Socket* socket){

        server->broadcast("login", { {"addr", socket->addr()} });

        socket->on("message", [server, socket](JSON data){
            server->broadcast("message", {{"addr", socket->addr()}, {"content", data}});
        });

        socket->on("disconnect", [server, socket](JSON){
            server->broadcast("leave", {{"addr", socket->addr()}});
        });

        socket->on("close-server", [server](JSON){
            server->close();
        });

    });

    server->listen(8000, 200);

    return 0;
}

