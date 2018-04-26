#ifndef _OPENCOG_COMMANDER_RpcSyncExecutor_H
#define _OPENCOG_COMMANDER_RpcSyncExecutor_H

#include "WebSocketIO/Server.h"
#include <opencog/atomspace/AtomSpace.h>
#include <map>

using namespace std;
namespace opencog
{

class RpcSyncExecutor
{
  public:
    RpcSyncExecutor(Socket *socket);
    ~RpcSyncExecutor();
    const string &call(const string &method, const string &params);

  private:
    Socket *_socket;
    map<string, string> _result_map = {};
};

} // namespace opencog
#endif  