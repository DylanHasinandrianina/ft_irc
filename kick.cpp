#include "kick.hpp"
#include "replybuilder.hpp"
#include "channelmanager.hpp"
#include "channels.hpp"

void Kick::execute(const Command& cmd, User& user, Server& serv)
{
	ReplyBuilder r;

	if (!user.isAuthenticated())
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOTREGISTERED, "*", "", ""));
		return;
	}

	if (cmd.paramCount() < 2)
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NEEDMOREPARAMS,
						   user.getNickname(),
						   "KICK",
						   ""));
		return;
	}

	ChannelManager& manager = serv.getChannelManager();

	std::string channelName = cmd.getParam(0);
	std::string targetNick  = cmd.getParam(1);

	if (!manager.hasChannel(channelName))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOSUCHCHANNEL,
						   user.getNickname(),
						   channelName,
						   ""));
		return;
	}

	Channel* channel = manager.findChannel(channelName);

	if (!channel->isOperator(&user))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_CHANOPRIVSNEEDED,
						   user.getNickname(),
						   channelName,
						   ""));
		return;
	}

	User* target = serv.findClientByNick(targetNick);
	if (!target)
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOTONCHANNEL,
						   user.getNickname(),
						   targetNick,
						   channelName));
		return;
	}

	// optional reason
	// ex : KICK #chan user :reason text
	std::string reason = cmd.getTrailing();

	std::string kickMsg =
		":" + user.getNickname() +
		"!" + user.getUsername() +
		"@localhost KICK " +
		channelName + " " +
		targetNick;

	if (!reason.empty())
		kickMsg += " :" + reason;

	kickMsg += "\r\n";

	channel->broadcast(kickMsg);

	channel->removeUser(target);
	channel->removeOperator(target);

	if (channel->isEmpty())
		manager.removeChannel(channelName);
}
