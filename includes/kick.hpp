#ifndef KICK_HPP
#define KICK_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Kick
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
