#include "join.hpp"
#include "replybuilder.hpp"
#include "channelmanager.hpp"
#include "channels.hpp"
#include "user.hpp"

#include <map>
#include <string>

static std::string buildNames(Channel& channel)
{
	std::string names;

	const std::map<int, User*>& users = channel.getUsers();

	for (std::map<int, User*>::const_iterator it = users.begin();
		 it != users.end();
		 ++it)
	{
		User* u = it->second;
		if (!u)
			continue;

		if (channel.isOperator(u))
			names += "@";

		names += u->getNickname();
		names += " ";
	}

	//remove last space 
	if (!names.empty())
		names.erase(names.size() - 1);

	return names;
}

void Join::execute(const Command& cmd, User& user, Server& serv)
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
			r.NumericReply(ERR_NEEDMOREPARAMS, user.getNickname(), "JOIN", ""));
		return;
	}

	std::string channelName = cmd.getParam(0);

	if (channelName.empty() || channelName[0] != '#')
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOSUCHCHANNEL, user.getNickname(), channelName, ""));
		return;
	}

	ChannelManager& manager = serv.getChannelManager();

	Channel* channel;

	if (manager.hasChannel(channelName))
		channel = manager.findChannel(channelName);
	else
		channel = &manager.createChannel(channelName);

	if (channel->isUserInChannel(&user))
		return;

	if (channel->isInviteOnlyMode() && !user.isInvited(channelName))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_INVITEONLYCHAN,
						   user.getNickname(),
						   channelName,
						   ""));
		return;
	}
	bool firstUser = channel->isEmpty();


	channel->addUser(&user);
	if (firstUser)
		channel->addOperator(&user);

	std::string joinMsg =
		":" + user.getNickname() +
		"!" + user.getUsername() +
		"@localhost JOIN " +
		channelName + "\r\n";
	channel->broadcast(joinMsg);
	// RFC requires those because clients assume it will be sent
	std::string names = buildNames(*channel);
	user.appendOutBuffer(
		r.NumericReply(RPL_NAMREPLY, user.getNickname(), "=" + channelName, names));

	user.appendOutBuffer(
		r.NumericReply(RPL_ENDOFNAMES, user.getNickname(), channelName, "End of NAMES"));
	
	//add fix topic
	if (!channel->getTopic().empty())
		user.appendOutBuffer(
			r.NumericReply(RPL_TOPIC, user.getNickname(), channelName, channel->getTopic()));
}
