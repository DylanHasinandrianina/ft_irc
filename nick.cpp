#include "nick.hpp"
#include "replybuilder.hpp"
#include "command.hpp"
#include "user.hpp"

void Nick::execute(const Command& cmd, User& user, Server &serv)
{
	// if(!user.isPassOk())
	// 	return ;
    ReplyBuilder r;

    if (cmd.paramCount() < 1)
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NONICKNAMEGIVEN, "*", "No nickname given", ""));
        return;
    }

    std::string newNick = cmd.getParam(0);

    // basic validation
    if (newNick.empty() || newNick.find(' ') != std::string::npos)
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_ERRONEUSNICKNAME, newNick, "Erroneous nickname", ""));
        return;
    }

	if (serv.findClientByNick(newNick))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NICKNAMEINUSE, user.getNickname(), newNick,
				  "Nickname is already in use"));
		return;
	}

	 std::string oldNick = user.getNickname();

    user.setNickname(newNick);
    user.setNickOk(true);

    std::string msg =
        ":" + oldNick + "!" + user.getUsername() +
        "@localhost NICK :" + newNick + "\r\n";
	serv.getChannelManager().broadcastToUser(user, msg);
	serv.tryRegister(user);
}
