#include "server.hpp"
#include "dispatch.hpp"
#include "command.hpp"
#include "replybuilder.hpp"

#include <unistd.h>

// volatile : no optimization
// sig_atomic_t : cannot be half written/interrupted = read/write atomically
volatile sig_atomic_t sig = 1;

void handle_signal(int)
{
	sig = 0;
}

Server::Server(const int port, const std::string password)
	: 	_ServerFd(-1),
		_Port(port),
		_Password(password),
		_Dispatch(),
		_ChannelManager()
{
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	std::cout << "Server running\n";

}

Server::~Server()
{
	CleanServer();
	std::cout << "Server shutdown\n";
}

const std::string& Server::getPassword() const
{
    return _Password;
}

		// --- Client access ---
bool Server::HasClient(int fd) const
{
    return _Clients.find(fd) != _Clients.end();
}

User& Server::GetClient(int fd)
{
    return _Clients.at(fd);
}

void Server::RemoveClient(int fd)
{
    _Clients.erase(fd);
}

User* Server::findClientByNick(const std::string& nick)
{
    for (std::map<int, User>::iterator it = _Clients.begin();
         it != _Clients.end();
         ++it)
    {
        if (it->second.getNickname() == nick)
            return &it->second;
    }
    return NULL;
}

		// --- Core functions ---
bool Server::InitServer()
{
	// PF_INET : IPv4, PF : protocol family
	// SOCK_STREAM : socket type for TCP
	// 0 : default protocol for this
	_ServerFd = socket(PF_INET, SOCK_STREAM, 0);
	if (_ServerFd == -1)
	{
		std::cerr << "Error socket :" << strerror(errno) << "\n";
		return (false);
	}
	/* Make the address unique and let the addr/port be reusable*/
	int opt = 1;
	if (setsockopt(_ServerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		return false;
	//
	sockaddr_in addr;

	// AF : adress family
	// htons because network uses big endian
	// INADDR_ANY : 0.0.0.0, accept all connections
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_Port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_ServerFd, (sockaddr *)&addr, sizeof(addr)) == -1)
	{
		std::cerr << "Error bind :" << strerror(errno) << "\n";
		return (false);
	}
	// SOMAXCONN is max backlog size, often 128 pending connections
	if (listen(_ServerFd, SOMAXCONN) == -1)
	{
		std::cerr << "Error listen :" << strerror(errno) << "\n";
		return (false);
	}
	if (fcntl(_ServerFd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error fcntl :" << strerror(errno) << "\n";
		return (false);
	}
	pollfd	Pfd;

	Pfd.fd = _ServerFd;
	// POLLIN means we monitor and wake when something to read in socket pops
	Pfd.events = POLLIN;
	// initialize to 0 just for satefy, revents is what happened
	Pfd.revents = 0;
	_Fds.push_back(Pfd);
	return (true);
}

void Server::ServerLoop()
{
	while (sig)
	{
		CheckIfClientHasSomethingTowrite();

			if (!poll(&_Fds[0], _Fds.size(), -1))
			{
				if (errno == EINTR)
					continue ;
				std::cerr << "Error poll :" << strerror(errno) << "\n";
				continue ;
			}
		for (size_t i = 0; i < _Fds.size();)
		{
			int CurrFd = _Fds[i].fd;
			int	revents = _Fds[i].revents;

			bool disconnect = false;

			if ((revents & POLLIN) && CurrFd == _ServerFd)
			{
					AcceptClient();
					 _Fds[i].revents = 0;
					i++;
					continue ;
			}
			else if (CurrFd != _ServerFd)
			{
				// POLLHUP : hang up, POLLER : socket level error, POLLNVAL : invalid fd
				if (revents & (POLLHUP | POLLERR | POLLNVAL))
				{
					ClientDisconnect(CurrFd);
					continue ;
				}
				if (revents & POLLIN)
				{
	   				if (!ReceiveFromClient(CurrFd))
						disconnect = true;
				}
				if (revents & POLLOUT)
				{
					if (!SendToClient(CurrFd))
						disconnect = true;
				}
				if (disconnect || GetClient(CurrFd).isDisconnected())
				{
					ClientDisconnect(CurrFd);
					continue ;
				}
			}
			_Fds[i].revents = 0;
			i++;
		}
	}
}

//		--- IO ---
// We enable and disable POLLOUT manually
// to avoid poll wake ups and waste CPU
// POLLOUT means socket is ready for writing
void	Server::EnableWrite(int fd)
{
	for (size_t i = 0; i < _Fds.size(); i++)
	{
	  if (_Fds[i].fd == fd)
	  {
			//POLLOUT is 0010, | is bitwise OR
			//we use bitwise so events can store
			//multiple flags as masks
			//so here | adds the bit of POLLOUT flag
			 if (!(_Fds[i].events & POLLOUT))
				_Fds[i].events |= POLLOUT;
	  		break;
	  }
	}
}

void	Server::DisableWrite(int fd)
{
	for (size_t i = 0; i < _Fds.size(); i++)
	{
	  if (_Fds[i].fd == fd)
	  {
			// ~POLLOUT is reverse of POLLOUT so 1101
			// as & is AND bitwise
			// it removes POLLOUT bitmask and only that
	  		_Fds[i].events &= ~POLLOUT;
	  		break;
	  }
	}
}

// Checks all client socket and enables POLLOUT
// if the user has pending data in their outgoing buffer
void	Server::CheckIfClientHasSomethingTowrite()
{
	for (size_t i = 0; i < _Fds.size(); i++)
	{
		int fd = _Fds[i].fd;

  		if (fd == _ServerFd)
  			continue;

		if (HasClient(fd))
		{
			User &c = GetClient(fd);

			if (c.hasOutBuffer() && !(_Fds[i].events & POLLOUT))
				EnableWrite(fd);
		}
	}
}

// Sends pending data from user outgoing buffer
// It sends only the bytes successfully written and keep the rest for later sends
bool Server::SendToClient(int fd)
{
    User& user = GetClient(fd);

    std::string& out = const_cast<std::string&>(user.getOutBuffer());

    if (out.empty())
    {
        DisableWrite(fd);
        return true;
    }

    ssize_t sent = send(fd, out.c_str(), out.size(), 0);
    if (sent == 0)
        return false;
	if (sent == -1)
		{
			//busy but no disconnect
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return true; 
			//interrupted but no disconnect
			if (errno == EINTR)
				return true;
			return false;
		}
	out.erase(0, sent);
    if (out.empty())
        DisableWrite(fd);

    return true;
}

// Reads data from client sockets and appends it to their input buffer
// Extract full IRC message and dispatch it to command handler
bool Server::ReceiveFromClient(int fd)
{
    char buffer[1024];

    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	// It means disconnect
    if (bytes == 0)
        return false;
if (bytes == -1)
{
    if (errno == EAGAIN || errno == EWOULDBLOCK)
        return true; // NOT a disconnect
	if (errno == EINTR)
		return true;
    return false;
}

    buffer[bytes] = '\0';

    if (!HasClient(fd))
        return false;

    User& user = GetClient(fd);
	std::cout << "RAW RECV [" << bytes << "] = ";
	std::cout.write(buffer, bytes);
	std::cout << "]" << std::endl;
    // append raw data to INPUT buffer
    user.appendToBuffer(std::string(buffer));

	std::string	line;
	Command		cmd;

	// extractcommand : extract one complete IRC message line form input buffer delimited by \n
	// ParseCommand : parse line into prefix, command, params, trailing
	while (user.extractCommand(line))
	{
			std::cout << "LINE: [" << line << "]" << std::endl;
		cmd.ParseCommand(line);

		if (!cmd.isValid())
			continue ;

		_Dispatch.dispatch(cmd, user, *this);
	}

    return true;
}

//		--- Connection ---
void	Server::AcceptClient()
{
	sockaddr_in	ClientAddr;
	socklen_t	Len = sizeof(ClientAddr);

	// Like bind, second argument is old school polymorphism, size passed by pointer
	// because accept can modify it
	int	ClientFd = accept(_ServerFd, (sockaddr*)&ClientAddr, &Len);
	// EAGAIN : ressource temporarily unavaible
	// EWOULDBLOCK : Operation would block
	// both are false errors
	if (ClientFd == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		std::cerr << "Error accept :" << strerror(errno) << "\n";
		return ;
	}
	if (fcntl(ClientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(ClientFd);
		std::cerr << "Error fcntl :" << strerror(errno) << "\n";
		return ;
	}
	
	pollfd	Pfd;

	Pfd.fd = ClientFd;
	Pfd.events = POLLIN;
	Pfd.revents = 0;
	_Fds.push_back(Pfd);

	_Clients.insert(std::make_pair(ClientFd, User(ClientFd)));

	std::cout << "New client connected, fd :" << ClientFd << "\n";
}

void Server::ClientDisconnect(int fd)
{
    for (size_t i = 0; i < _Fds.size(); i++)
    {
        if (_Fds[i].fd == fd)
        {
            _Fds.erase(_Fds.begin() + i);
            break;
        }
    }

    RemoveClient(fd);
    close(fd);
}

void Server::markDisconnect(int fd)
{
    if (HasClient(fd))
        GetClient(fd).setDisconnected(true);
}

void	Server::CleanServer()
{
	for (std::map<int, User>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
	{
		  close(it->first);
	}

	_Clients.clear();
	_Fds.clear();

	if (_ServerFd != -1)
	 close (_ServerFd);
}

void Server::tryRegister(User& user)
{
	ReplyBuilder r;

	std::cout
    << "PASS:" << user.isPassOk()
    << " NICK:" << user.isNickOk()
    << " USER:" << user.isUserOk()
    << std::endl;

    if (user.isAuthenticated())
        return;

    if (user.isPassOk() && user.isNickOk() && user.isUserOk())
    {
        user.setAuthenticatedOk(true);

	  user.appendOutBuffer(
            r.NumericReply(RPL_WELCOME,
                           user.getNickname(),
                           ":Welcome to the IRC network",
                           ""));
        user.appendOutBuffer(
            r.NumericReply(RPL_YOURHOST,
                           user.getNickname(),
                           ":Your host is ircserv, running version 1.0",
                           ""));
        user.appendOutBuffer(
            r.NumericReply(RPL_CREATED,
                           user.getNickname(),
                           ":This server was created today",
                           ""));
        user.appendOutBuffer(
            r.NumericReply(RPL_MYINFO,
                           user.getNickname(),
                           "ircserv 1.0 o o",
                           ""));

	user.setAuthenticatedOk(true);
	}
}

ChannelManager& Server::getChannelManager()
{
    return _ChannelManager;
}
