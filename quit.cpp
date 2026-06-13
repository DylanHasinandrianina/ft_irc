#include "server.hpp"
#include "command.hpp"
#include "user.hpp"
#include "channelmanager.hpp"
#include "quit.hpp"
#include "replybuilder.hpp"
#include "channels.hpp"

void Quit::execute(const Command& cmd, User& user, Server& serv)
{
    ReplyBuilder r;

	// standard reason or with or without :
    std::string reason = "Client Quit";
    if (cmd.paramCount() > 0)
        reason = cmd.getTrailing().empty() ? cmd.getParam(0) : cmd.getTrailing();

    std::string quitMsg =
        ":" + user.getNickname() +
        "!user@localhost QUIT :" +
        reason + "\r\n";

    ChannelManager& manager = serv.getChannelManager();

    std::map<std::string, Channel>& channels = manager.getChannels();

    for (std::map<std::string, Channel>::iterator it = channels.begin();
         it != channels.end();
         ++it)
    {
        Channel& channel = it->second;

        if (channel.isUserInChannel(&user))
        {
            channel.broadcast(quitMsg);
            channel.removeUser(&user);
            channel.removeOperator(&user);
        }
    }

    serv.markDisconnect(user.getFd());
}
