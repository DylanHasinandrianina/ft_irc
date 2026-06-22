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

    for (size_t i = 0; i < subcmd.size(); i++)
        subcmd[i] = std::toupper((unsigned char)subcmd[i]);

    if (subcmd == "LS")
    {
        user.appendOutBuffer(":ircserv CAP * LS :\r\n");
    }
    else if (subcmd == "REQ")
    {
        std::string caps = cmd.getTrailing();
        user.appendOutBuffer(":ircserv CAP * NAK :" + caps + "\r\n");
    }
    else if (subcmd == "END")
    {
        serv.tryRegister(user);
    }
}