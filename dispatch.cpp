#include "replybuilder.hpp"
#include "dispatch.hpp"
#include "command.hpp"
#include "server.hpp"
#include "user.hpp"
#include "pass.hpp"
#include "nick.hpp"
#include "usercmd.hpp"
#include "join.hpp"
#include "part.hpp"
#include "privmsg.hpp"
#include "kick.hpp"
#include "invite.hpp"
#include "topic.hpp"
#include "mode.hpp"
#include "quit.hpp"
#include "cap.hpp"

Dispatcher::Dispatcher() {}

Dispatcher::~Dispatcher() {}

void Dispatcher::dispatch(const Command& command, User& client, Server& serv)
{
    const std::string& cmd = command.getCmd();

	std::cout << "[DISPATCH] cmd=" << cmd << std::endl;

    if (cmd == "CAP")
        return Cap::execute(command, client, serv);

    if (cmd == "PASS")
        return Pass::execute(command, client, serv);

    if (cmd == "NICK")
        return Nick::execute(command, client, serv);

    if (cmd == "USER"){
        return UserCmd::execute(command, client, serv);
    }

	if (cmd == "JOIN")
	       return Join::execute(command, client, serv);

    if (cmd == "PRIVMSG")
        return Privmsg::execute(command, client, serv);

    if (cmd == "KICK")
        return Kick::execute(command, client, serv);

    if (cmd == "PART")
        return Part::execute(command, client, serv);

    if (cmd == "INVITE")
        return Invite::execute(command, client, serv);

    if (cmd == "TOPIC")
        return Topic::execute(command, client, serv);

    if (cmd == "MODE")
        return Mode::execute(command, client, serv);

    if (cmd == "QUIT"){
        return Quit::execute(command, client, serv);
    }

	ReplyBuilder r;

	// client.appendOutBuffer(
	// 	r.NumericReply(ERR_UNKNOWNCOMMAND, client.getNickname(), cmd, "Unknown command"));
}
