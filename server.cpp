#include "./includes/server.hpp"
#include "./includes/command.hpp"
#include <sys/socket.h> // socket() listen() send()
#include <fcntl.h> // fcntl()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>   // close()

Server::Server(int port, std::string password) : _port(port), _password(password){}

Server::~Server(){
    cleanup();
}

int Server::initServer(){
    //ouvre le socket du serveur
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd == -1){
        std::cerr << "Error socket server\n";
        return 0;
    }
    // pour que le serveur soit non bloquant
    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1){
        std::cerr << "Error fcntl server\n";
        close(_serverFd);
        return 0;
    }

    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR,
                &opt, sizeof(opt)) == -1)
    {
        std::cerr << "setsockopt failed\n";
        close(_serverFd);
        return 0;
    }

    //remplir la structure necessaire pour bind apres
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);
    
    //lier le socket serveur a un port
    int ret = bind(_serverFd, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret == -1){
        std::cerr << "Error bind\n";
        close(_serverFd);
        return 0;
    }

    //attendre les connections
    int retli = listen(_serverFd, 10);
    if (retli == -1){
        std::cerr << "Error listen\n";
        close(_serverFd);
        return 0;
    }

    //remplir la structure necessaire pour poll apres
    pollfd servPoll;
    servPoll.fd = _serverFd;
    servPoll.events = POLLIN;
    servPoll.revents = 0;
    _pollFds.push_back(servPoll);

    return 1;
}

void Server::cleanup()
{
    for (size_t i = 0; i < _pollFds.size(); i++){
        close(_pollFds[i].fd);
    }
    _pollFds.clear();
    std::map<int, User*>::iterator it = _users.begin();
    while (it != _users.end())
    {
        delete it->second;
        ++it;
    }
    //channels
    std::map<std::string, Channel*>::iterator itChan = _channels.begin();
    while (itChan != _channels.end())
    {
        delete itChan->second;
        ++itChan;
    }
    _channels.clear();
    _users.clear();
}

int Server::serverEvent()
{
    sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    int newClientfd = accept(_serverFd, (sockaddr *)&clientAddr, &len);
    if (newClientfd == -1)
    {
        std::cerr << "Error accept\n";
        return 0;
    }
    //on met le client dans la map qui contient tout les client du serveur
    //mais il n'est vraiment membre qu'apres authentification
    _users[newClientfd] = new User(newClientfd);
    if (fcntl(newClientfd, F_SETFL, O_NONBLOCK) == -1){
        delete _users[newClientfd];
        _users.erase(newClientfd);
        close(newClientfd);
        return 0;
    }

    pollfd clientPoll;
    clientPoll.fd = newClientfd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    _pollFds.push_back(clientPoll);

    return 1;
}

// void Server::ExecuteCommand(Command& cmd)
// {
//     if (cmd.command.empty())
//         return;
    
//     if (cmd.command == "PASS")
//         handlePass(cmd);
// }

void Server::disconnectClient(int fd)
{
    // récupérer le User
    std::map<int, User*>::iterator userIt = _users.find(fd);
    if (userIt == _users.end())
        return;

    User* user = userIt->second;

    // retirer le user de tous les channels
    for (std::map<std::string, Channel*>::iterator it = _channels.begin();
         it != _channels.end(); )
    {
        Channel* channel = it->second;

        channel->removeUser(user);
        channel->removeOperator(user);

        // supprimer le channel s'il devient vide
        if (channel->getNbUser() == 0)
        {
            delete channel;
            _channels.erase(it++);
        }
        else
            ++it;
    }

    // fermer le socket
    close(fd);

    // supprimer le User
    delete user;
    _users.erase(fd);

    // marquer le pollfd comme invalide
    for (size_t i = 0; i < _pollFds.size(); i++)
    {
        if (_pollFds[i].fd == fd)
        {
            _pollFds[i].fd = -1;
            break;
        }
    }
}

void Server::clientEvent(size_t i)
{
    char buffer[1025];
    int fd = _pollFds[i].fd;
    int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes <= 0){
        disconnectClient(fd);
        std::cout << "Client " << fd << " disconnected\n";
        return;
    }
    else
    {
        buffer[bytes] = '\0';
        User *user = _users[fd];
        user->appendToBuffer(buffer);
        std::string line;
        while ((user->extractCommand(line))){
            Command cmd = parseCommand(line);
            std::cout << "command name : " << cmd.command << '\n';
            for (size_t j = 0; j < cmd.params.size(); j++){
                std::cout << "param [" << j << "] : "<< cmd.params[j] << '\n';
            }

            //handle command
        }

    }
}

void Server::run(){
    if (!initServer())
        return ;
    while(g_running)
    {
        //surveille le serveur et les clients
        int retp = poll(_pollFds.data(), _pollFds.size(), -1);
        if (retp == -1){
            cleanup();
            return;
        }
        //parcourir le pollfds pour verifier si il y a un evenement dans un socket
        for (size_t i = 0; i < _pollFds.size(); i++){
            if (_pollFds[i].revents & POLLIN){
                //si l'evenement est du cote du serveur, ca veut dire qu'un client
                //veut se connecter
                if (_pollFds[i].fd == _serverFd){
                    if (!serverEvent())
                        continue;
                }
                //si l'evenement est du cote du client, ca ve dire qu'il veut envoyer un message
                else
                {
                    clientEvent(i);
                    if (i < _pollFds.size() && (_pollFds[i].fd == -1)){
                        _pollFds.erase(_pollFds.begin() + i);
                        if (i > 0)
                            i--;
                    }
                }
            }
        }
    }
}