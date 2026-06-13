#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Privmsg
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
