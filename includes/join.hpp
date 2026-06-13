#ifndef JOIN_HPP
#define JOIN_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Join
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
