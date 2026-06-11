#ifndef SERVER_HPP

#define SERVER_HPP

# include <string>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include <map>
# include <iostream>

# include "user.hpp"
# include "dispatch.hpp"

class Dispatcher;

class Server
{
	private:
		int						_ServerFd;
		int						_Port;
		std::vector<pollfd>		_Fds;
		std::map<int, User>		_Clients;
		const std::string		_Password;
		Dispatcher				_Dispatch;
	public:
		Server(const int port, const std::string password);
		~Server();

		const std::string &getPassword() const;
		// --- Client access ---
		bool					HasClient(int fd) const;
		User 					&GetClient(int fd);
		User*					findClientByNick(const std::string& nick);

		void					RemoveClient(int fd);

		// --- Core functions ---
		bool 					InitServer();
		void					ServerLoop();

		// --- IO ---
		void					EnableWrite(int fd);
		void					DisableWrite(int fd);
		void					CheckIfClientHasSomethingTowrite();
		bool					SendToClient(int fd);
		bool					ReceiveFromClient(int fd);

		// --- Connection ---
		void					AcceptClient();
		void					ClientDisconnect(int fd);
		void					CleanServer();

		void					tryRegister(User& user);
};

#endif
