#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <string>

class Command;
class User;
class Server;
class Pass;

class Dispatcher
{
    public:
        Dispatcher();
        ~Dispatcher();

        void dispatch(const Command& command, User& client, Server& serv);
};

#endif
