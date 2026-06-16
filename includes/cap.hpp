#ifndef CAP_HPP
#define CAP_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Cap
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif