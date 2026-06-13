#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Nick
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};
