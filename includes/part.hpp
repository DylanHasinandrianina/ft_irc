#ifndef PART_HPP
#define PART_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Part
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
