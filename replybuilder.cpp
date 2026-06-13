#include "replybuilder.hpp"

#include <sstream>

ReplyBuilder::ReplyBuilder() { _serverName = "ircserv"; }
ReplyBuilder::~ReplyBuilder() {}

std::string ReplyBuilder::formatReply(ReplyCode code,
                                const std::string& nick,
                                const std::string& target,
                                const std::string& extra)
{
	std::ostringstream out;

	out << ":" << _serverName << " "
		<< code << " "
		<< (nick.empty() ? "*" : nick);

	if (!target.empty())
		out << " " << target;

	if (!extra.empty())
		out << " :" << extra;

	out << "\r\n";

	return out.str();
}

// :server <code> <nick> <target> :<message>
// target is optional, message is optional
std::string ReplyBuilder::NumericReply
	(ReplyCode code,
	const std::string &nick,
	const std::string &target,
	const std::string &message)
{
	switch (code)
	{
		// ======================
		// connection / welcome
		// ======================
		case RPL_WELCOME:
			return formatReply(code, nick, "",
							   "Welcome to the IRC server");

		case RPL_YOURHOST:
			return formatReply(code, nick, "",
							   "Your host is " + _serverName);

		case RPL_CREATED:
			return formatReply(code, nick, "",
							   "This server was created today");


		case RPL_MYINFO:
			return formatReply(code, nick, "",
							   "ircserv 1.0 o o");

		// ======================
		// registration errors
		// ======================
		case ERR_NONICKNAMEGIVEN:
			return formatReply(code, "*", "",
							   "No nickname given");

		case ERR_ERRONEUSNICKNAME:
			return formatReply(code, nick, target,
							   "Erroneous nickname");

		case ERR_NICKNAMEINUSE:
			return formatReply(code, "*", target,
							   "Nickname is already in use");

		case ERR_NOTREGISTERED:
			return formatReply(code, nick, "",
							   "You have not registered");

		case ERR_NEEDMOREPARAMS:
			return formatReply(code, nick, target,
							   "Not enough parameters");

		case ERR_ALREADYREGISTERED:
			return formatReply(code, nick, "",
							   "You may not reregister");

		case ERR_PASSWDMISMATCH:
			return formatReply(code, "*", "",
							   "Password incorrect");

		// ======================
		// channel errors
		// ======================
		case ERR_NOSUCHCHANNEL:
			return formatReply(code, nick, target,
							   "No such channel");

		case ERR_CANNOTSENDTOCHAN:
			return formatReply(code, nick,target,
							   "Cannot send to channel");

		case ERR_CHANNELISFULL:
			return formatReply(code, nick, target,
							   "Cannot join channel (+l)");

		case ERR_INVITEONLYCHAN:
			return formatReply(code, nick, target,
							   "Cannot join channel (+i)");

		case ERR_BADCHANNELKEY:
			return formatReply(code, nick, target,
							   "Cannot join channel (+k)");

		case ERR_CHANOPRIVSNEEDED:
			return formatReply(code, nick, target,
							   "You're not channel operator");

		case ERR_NOTONCHANNEL:
			return formatReply(code, nick, target,
							   "You're not on that channel");

		// ======================
		// message errors
		// ======================
		case ERR_NORECIPIENT:
			return formatReply(code, nick, "",
							   "No recipient given");

		case ERR_NOTEXTTOSEND:
			return formatReply(code, nick, "",
							   "No text to send");

		case ERR_UNKNOWNCOMMAND:
			return formatReply(code, nick, target,
							   "Unknown command");

		// ======================
		// channel replies
		// ======================
		case RPL_NOTOPIC:
			return formatReply(code, nick, target,
							   "No topic is set");
		case RPL_TOPIC:
			return formatReply(code, nick, target, message);

		case RPL_INVITING:
			return formatReply(code, nick, target, "has been invited");

		case RPL_NAMREPLY:
			return formatReply(code, nick, target, message);

		case RPL_ENDOFNAMES:
			return formatReply(code, nick, target,
							   "End of /NAMES list");

		default:
			return formatReply(code, nick, "",
							   "Unknown reply");
	}
}
