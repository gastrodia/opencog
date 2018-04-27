#ifndef _OPENCOG_COMMANDER_RpcSyncExecutor_H
#define _OPENCOG_COMMANDER_RpcSyncExecutor_H

#include "WebSocketIO/Server.h"

using namespace std;
namespace opencog
{

class RpcSyncExecutor
{
public:
  RpcSyncExecutor(Server *server,Socket *socket);
  ~RpcSyncExecutor();
  string call(const string &method, const string &params);

private:
  Socket *_socket;
  Server *_server;
  string get_task_record_file(string &guid);
};

} // namespace opencog
#endif