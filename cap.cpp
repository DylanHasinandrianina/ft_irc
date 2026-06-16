#include "cap.hpp"
#include "replybuilder.hpp"
#include "command.hpp"
#include "user.hpp"
#include "server.hpp"

void Cap::execute(const Command& cmd, User& user, Server& serv)
{
    (void)serv;

    if (cmd.paramCount() < 1)
        return;

    std::string subcmd = cmd.getParam(0);

    // Convertir en majuscules
    for (size_t i = 0; i < subcmd.size(); i++)
        subcmd[i] = std::toupper((unsigned char)subcmd[i]);

    if (subcmd == "LS")
    {
        // On annonce qu'on n'a aucune capability
        user.appendOutBuffer(":ircserv CAP * LS :\r\n");
    }
    else if (subcmd == "REQ")
    {
        // On rejette toute demande de capability
        std::string caps = cmd.getTrailing();
        user.appendOutBuffer(":ircserv CAP * NAK :" + caps + "\r\n");
    }
    else if (subcmd == "END")
    {
        // Le client a fini la négociation, on peut essayer de register
        serv.tryRegister(user);
    }
    // LIST, ACK etc. : on ignore
}