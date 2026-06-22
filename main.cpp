#include "server.hpp"
#include <sys/socket.h> // socket() listen() send()
#include <fcntl.h> // fcntl()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>   // close()
#include <cstdlib> //atoi
#include <csignal>

bool g_running = true;

void signalHandler(int sig)
{
    (void)sig;
    g_running = false;
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>\n";
        return (1);
    }
    // ports 0 -> 1023: system services (SSH, HTTP, HTTPS, ...)
    // ports 1024 -> 49151: user ports, adapt for the project
    // ports 49152 -> 65535: dynamic/ephemeral ports for temporary connections
    int port = atoi(av[1]);
    if (port < 1024 || port > 65535)
    {
        std::cout << "Please chose a port between 1024 - 65535\n";
        return (1);
    }
    std::string password = av[2];
    
    signal(SIGINT, signalHandler);

    Server server(port, password);
    if (!server.InitServer()){
        return 1;
    }
	server.ServerLoop();

    return 0;

}
