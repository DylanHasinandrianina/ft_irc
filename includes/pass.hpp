#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Pass
{
public:
    static void execute(const Command& cmd, User& user, Server &serv);
};
