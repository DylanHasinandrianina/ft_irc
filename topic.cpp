#include "topic.hpp"
#include "replybuilder.hpp"
#include "channelmanager.hpp"
#include "channels.hpp"

void Topic::execute(const Command& cmd, User& user, Server& serv)
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
			r.NumericReply(ERR_NEEDMOREPARAMS,
						   user.getNickname(),
						   "TOPIC",
						   ""));
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

	if (cmd.paramCount() == 1 && cmd.getTrailing().empty())
	{
		if (channel->getTopic().empty())
		{
			user.appendOutBuffer(
				r.NumericReply(RPL_NOTOPIC,
							   user.getNickname(),
							   channelName,
							   ":No topic is set"));
		}
		else
		{
			user.appendOutBuffer(
				r.NumericReply(RPL_TOPIC,
							   user.getNickname(),
							   channelName,
							   channel->getTopic()));
		}
		return;
	}

	if (channel->isTopicOpMode() && !channel->isOperator(&user))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_CHANOPRIVSNEEDED,
						   user.getNickname(),
						   channelName,
						   ""));
		return;
	}

	std::string newTopic = cmd.getTrailing();
	if (newTopic.empty() && cmd.paramCount() > 1)
	{
		newTopic = cmd.getParam(1);
		for (size_t i = 2; i < cmd.paramCount(); i++)
		{
			newTopic += " " + cmd.getParam(i);
		}
	}

	channel->setTopic(newTopic);

std::cout << "[TOPIC] SET on channel " << channel
          << " value='" << newTopic << "'\n";
	std::string msg =
		":" + user.getNickname() +
		"!" + user.getUsername() +
		"@localhost TOPIC " +
		channelName +
		" :" + newTopic + "\r\n";

	channel->broadcast(msg);
}
