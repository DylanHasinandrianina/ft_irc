#ifndef INVITE_HPP
#define INVITE_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Invite
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
