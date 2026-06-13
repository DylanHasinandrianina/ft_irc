#ifndef MODE_HPP
#define MODE_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Mode
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
