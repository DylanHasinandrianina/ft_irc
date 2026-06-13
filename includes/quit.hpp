#ifndef QUIT_HPP
#define QUIT_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Quit
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
