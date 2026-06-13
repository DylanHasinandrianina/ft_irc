#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

class Topic
{
public:
    static void execute(const Command& cmd, User& user, Server& serv);
};

#endif
