#include "invite.hpp"
#include "replybuilder.hpp"
#include "channelmanager.hpp"
#include "channels.hpp"

void Invite::execute(const Command& cmd, User& user, Server& serv)
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
            r.NumericReply(ERR_NEEDMOREPARAMS, user.getNickname(), "INVITE", ""));
        return;
    }

    std::string targetNick = cmd.getParam(0);
    std::string channelName = cmd.getParam(1);

    ChannelManager& manager = serv.getChannelManager();

    if (!manager.hasChannel(channelName))
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NOSUCHCHANNEL, user.getNickname(), channelName, ""));
        return;
    }

    Channel* channel = manager.findChannel(channelName);

    if (!channel->isUserInChannel(&user))
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NOTONCHANNEL, user.getNickname(), channelName, ""));
        return;
    }

    if (!channel->isOperator(&user))
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_CHANOPRIVSNEEDED, user.getNickname(), channelName, ""));
        return;
    }

    User* target = serv.findClientByNick(targetNick);
    if (!target)
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NOSUCHNICK, user.getNickname(), targetNick, ""));
        return;
    }

	target->addInvite(channelName);

    target->appendOutBuffer(
        ":" + user.getNickname() + "!user@localhost INVITE " +
        targetNick + " " + channelName + "\r\n");

    user.appendOutBuffer(
        r.NumericReply(RPL_INVITING, user.getNickname(), targetNick + " " + channelName, ""));
}
