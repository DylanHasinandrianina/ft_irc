#include "mode.hpp"
#include "replybuilder.hpp"
#include "channelmanager.hpp"
#include "channels.hpp"

static void setInviteMode(Channel* ch, bool enable)
{
    ch->setInviteOnlyMode(enable);
}

static void setTopicMode(Channel* ch, bool enable)
{
    ch->setTopicOpMode(enable);
}

static void setKeyMode(Channel* ch, bool enable, const std::string& key = "")
{
    if (enable)
        ch->setPassword(key);
    else
        ch->setPassword("");
}

static void setLimitMode(Channel* ch, bool enable, int limit = 0)
{
    if (enable)
        ch->setUserLimit(limit);
    else
        ch->setUserLimit(0);
}

void Mode::execute(const Command& cmd, User& user, Server& serv)
{
	ReplyBuilder r;

	if (!user.isAuthenticated())
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOTREGISTERED, user.getNickname(), "", ""));
		return;
	}

	if (cmd.paramCount() < 1)
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NEEDMOREPARAMS, user.getNickname(), "MODE", ""));
		return;
	}

	std::string target = cmd.getParam(0);

	ChannelManager& cm = serv.getChannelManager();

	if (!cm.hasChannel(target))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_NOSUCHCHANNEL, user.getNickname(), target, ""));
		return;
	}

	Channel* ch = cm.findChannel(target);

	// If no mode argument : just display modes
	if (cmd.paramCount() == 1)
	{
		std::string modes = "+";
		if (ch->isInviteOnlyMode()) modes += "i";
		if (ch->isKeyRequiredMode()) modes += "k";
		if (ch->isTopicOpMode())     modes += "t";

		user.appendOutBuffer(
			r.NumericReply(RPL_CHANNELMODEIS, user.getNickname(), target, modes));
		return;
	}

	if (!ch->isOperator(&user))
	{
		user.appendOutBuffer(
			r.NumericReply(ERR_CHANOPRIVSNEEDED, user.getNickname(), target, ""));
		return;
	}

	std::string modes = cmd.getParam(1);
	bool adding = true;
	size_t paramIndex = 2;

	for (size_t i = 0; i < modes.size(); i++)
	{
		char m = modes[i];

		if (m == '+') { adding = true; continue; }
		if (m == '-') { adding = false; continue; }

		switch (m)
		{
			case 'i':
				setInviteMode(ch, adding);
				break;

			case 't':
				setTopicMode(ch, adding);
				break;

			case 'k':
				if (adding && paramIndex < cmd.paramCount())
					setKeyMode(ch, true, cmd.getParam(paramIndex++));
				else
					setKeyMode(ch, false);
				break;

			case 'l':
				if (adding && paramIndex < cmd.paramCount())
					setLimitMode(ch, true, atoi(cmd.getParam(paramIndex++).c_str()));
				else
					setLimitMode(ch, false);
				break;

			default:
				break;
		}
	}
}
