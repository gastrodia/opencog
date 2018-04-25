#ifndef _OPENCOG_COMMANDER_SYNC_RPC_H
#define _OPENCOG_COMMANDER_SYNC_RPC_H


#include "WebSocketIO/Server.h"
#include <opencog/atomspace/AtomSpace.h>


namespace opencog
{

    class RpcSyncExecutor
    {
        public:
            RpcSyncExecutor(Socket* socket);
            std::string& call(std:string& method, std:string& params);
        private:
            Socket* _socket;
    };

} // namespace opencog

