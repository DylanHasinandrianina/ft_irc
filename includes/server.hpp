#ifndef SERVER_HPP
#define SERVER_HPP

#include "channels.hpp"
#include "user.hpp"
#include "dispatch.hpp"
#include "command.hpp"
#include <vector>
#include <poll.h>
#include <map>

extern bool g_running;

class Dispatcher;

class Server{
    private:
        int _port;
        std::string _password;
        int _serverFd;

        std::vector<struct pollfd> _pollFds;

        std::map<int, User*> _users;

        std::map<std::string, Channel*> _channels;
    
		Dispatcher _disp;
    public :
        Server(int port, std::string password);
        ~Server();

        int initServer();
        void run();
        void cleanup();
        int serverEvent();
        void clientEvent(size_t i);
        void disconnectClient(int fd);
        
		const std::string& getPassword() const;	
        // void ExecuteCommand(Command& cmd);

        // void handlePass(Command& cmd);
        // void handleNick(Command& cmd);
        // void handleUser(Command& cmd);
        // void handleJoin(Command& cmd);
        // void handlePrivmsg(Command& cmd);



};


#endif
