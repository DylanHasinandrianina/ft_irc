#ifndef REPLYBUILDER_HPP
#define REPLYBUILDER_HPP

#include <string>

enum ReplyCode
{
    RPL_WELCOME = 1,
    RPL_YOURHOST = 2,
    RPL_CREATED = 3,
    RPL_MYINFO = 4,

    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CANNOTSENDTOCHAN = 404,

    ERR_NORECIPIENT = 411,
    ERR_NOTEXTTOSEND = 412,
    ERR_UNKNOWNCOMMAND = 421,

    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,

	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
    RPL_CHANNELMODEIS = 324,
	RPL_INVITING = 341,

    ERR_NOTREGISTERED = 451,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTERED = 462,
    ERR_PASSWDMISMATCH = 464,

    ERR_CHANNELISFULL = 471,
    ERR_UNKNOWNMODE = 472,
    ERR_INVITEONLYCHAN = 473,
    ERR_BADCHANNELKEY = 475,
    ERR_CHANOPRIVSNEEDED = 482,

	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
};

class ReplyBuilder
{
	public:
		ReplyBuilder();
		~ReplyBuilder();


		std::string formatReply(ReplyCode code,
                                const std::string& nick,
                                const std::string& target,
                                const std::string& extra);

		std::string NumericReply(
			ReplyCode code,
			const std::string &nick,
			const std::string &target,
			const std::string &message);

	private:
		std::string _serverName;
};

#endif
