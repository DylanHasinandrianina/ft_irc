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

    std::string nick = cmd.getParam(0);

    // basic validation
    if (nick.empty() || nick.find(' ') != std::string::npos)
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_ERRONEUSNICKNAME, nick, "Erroneous nickname", ""));
        return;
    }

	if (serv.findClientByNick(nick))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NICKNAMEINUSE, user.getNickname(), nick,
				  "Nickname is already in use"));
		return;
	}
    user.setNickname(nick);
    user.setNickOk(true);
	serv.tryRegister(user);
}
