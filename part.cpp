#include "part.hpp"
#include "replybuilder.hpp"
#include "channelmanager.hpp"
#include "channels.hpp"

void Part::execute(const Command& cmd, User& user, Server& serv)
{
	ReplyBuilder r;

	if (!user.isAuthenticated())
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOTREGISTERED, "*", "", ""));
		return;
	}

	if (cmd.paramCount() < 1)
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NEEDMOREPARAMS, user.getNickname(), "PART", ""));
		return;
	}

	ChannelManager& manager = serv.getChannelManager();
	std::string channelName = cmd.getParam(0);

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

	if (!channel->isUserInChannel(&user))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOTONCHANNEL,
							user.getNickname(),
							channelName,
							""));
		return;
	}

	// to give a message after part command
	// ex : part #chan : bye bye
	std::string reason = "";
	if (cmd.paramCount() > 1)
		reason = cmd.getParam(1);

	std::string partMsg =
		":" + user.getNickname() +
		"!" + user.getUsername() +
		"@localhost PART " +
		channelName;

	if (!reason.empty())
		partMsg += " :" + reason;

	partMsg += "\r\n";

	channel->broadcast(partMsg);

	channel->removeUser(&user);
	channel->removeOperator(&user);

	if (channel->isEmpty())
		manager.removeChannel(channelName);
}
