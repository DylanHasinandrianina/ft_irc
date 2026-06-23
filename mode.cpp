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
	{
        ch->setPassword(key);
		ch->setKeyRequiredMode(true);
	}
    else
	{
        ch->setPassword("");
		ch->setKeyRequiredMode(false);
	}
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

	if (target[0] != '#')
	{
		user.appendOutBuffer(":ircserv 221 " + user.getNickname() + " +\r\n");
		return;
	}

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
        ":ircserv 324 " + user.getNickname() + " " + target + " " + modes + "\r\n");
		return;
	}

	if (cmd.getParam(1)[0] != '+' && cmd.getParam(1)[0] != '-')
	{
		user.appendOutBuffer(
			":ircserv 368 " + user.getNickname() + " " + target + " :End of channel ban list\r\n");
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

	std::string appliedModes;
	std::string appliedParams;
	char currentSign = 0;

	for (size_t i = 0; i < modes.size(); i++)
	{
		char m = modes[i];

		if (m == '+')	
		{
			adding = true;
			currentSign = '+';
			continue ;
		}
		if (m == '-')	
		{
			adding = false;
			currentSign = '-';
			continue ;
		}
		switch (m)
		{
			case 'i':
				setInviteMode(ch, adding);
				if (appliedModes.empty() || appliedModes[appliedModes.size() - 1] != currentSign)
					appliedModes += currentSign;
				appliedModes += 'i';
				break;

			case 't':
				setTopicMode(ch, adding);
				if (appliedModes.empty() || appliedModes[appliedModes.size() - 1] != currentSign)
					appliedModes += currentSign;
				appliedModes += 't';
				break;

			case 'o':
			{
				if (paramIndex >= cmd.paramCount())
				{
					user.appendOutBuffer(
						r.NumericReply(ERR_NEEDMOREPARAMS,
									   user.getNickname(),
									   "MODE",
									   ""));
					return;
				}
				std::string nick = cmd.getParam(paramIndex++);
				User* target = serv.findClientByNick(nick);

				if (!target || !ch->isUserInChannel(target))
					return;

				if (adding)
					ch->addOperator(target);
				else
					ch->removeOperator(target);

				if (appliedModes.empty() || appliedModes[appliedModes.size() - 1] != currentSign)
					appliedModes += currentSign;
				appliedModes += 'o';
				appliedParams += " " + nick;
				break;
			}
			case 'k':
				if (adding)
				{
					if (paramIndex >= cmd.paramCount())
					{
						user.appendOutBuffer(
	  						r.NumericReply(ERR_NEEDMOREPARAMS,
                               user.getNickname(), "MODE",""));
	  					return ;
	  				}
					std::string key = cmd.getParam(paramIndex++);
					setKeyMode(ch, true, key);

					if (appliedModes.empty() || appliedModes[appliedModes.size() - 1] != currentSign)
						appliedModes += currentSign;
					appliedModes += 'k';
					appliedParams += " " + key;
				}
				else
				{
					setKeyMode(ch, false);

					if (appliedModes.empty() || appliedModes[appliedModes.size() - 1] != currentSign)
					appliedModes += currentSign;
					appliedModes += 'k';
				}
				break;

			case 'l':
				if (adding)
				{
					if (paramIndex >= cmd.paramCount())
					{
						user.appendOutBuffer(
						r.NumericReply(ERR_NEEDMOREPARAMS, user.getNickname(),"MODE", ""));
					return;
	  				}
					std::string limitStr = cmd.getParam(paramIndex++);
					setLimitMode(ch, true, atoi(limitStr.c_str()));

					if (appliedModes.empty() || appliedModes[appliedModes.size() - 1] != currentSign)
						appliedModes += currentSign;
					appliedModes += 'l';
					appliedParams += " " + limitStr;
				}
				else
				{
				   setLimitMode(ch, false);

					if (appliedModes.empty() || appliedModes[appliedModes.size() - 1] != currentSign)
						appliedModes += currentSign;
					appliedModes += 'l';
				}
				break;
			default:
				break;
		}
	}

	if (!appliedModes.empty())
	{
		std::string modeMsg =
			":" + user.getNickname() +
			"!" + user.getUsername() +
			"@localhost MODE " +
			target + " " + appliedModes + appliedParams + "\r\n";
		ch->broadcast(modeMsg);
	}
}
