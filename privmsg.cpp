#include "privmsg.hpp"
#include "replybuilder.hpp"
#include "channelmanager.hpp"
#include "channels.hpp"

void Privmsg::execute(const Command& cmd, User& user, Server& serv)
{
    ReplyBuilder r;

    if (!user.isAuthenticated())
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NOTREGISTERED, "*", "", ""));
        return;
    }

    // PRIVMSG <target> <message>
    if (cmd.paramCount() < 1)
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NEEDMOREPARAMS,
                           user.getNickname(),
                           "PRIVMSG",
                           ""));
        return;
    }

    std::string target = cmd.getParam(0);
    std::string message = cmd.getTrailing();

    // EMPTY MESSAGE CHECK
    if (message.empty())
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NOTEXTTOSEND,
                           user.getNickname(),
                           "",
                           ""));
        return;
    }

    ChannelManager& manager = serv.getChannelManager();

    // CHANNEL MESSAGE
    if (!target.empty() && target[0] == '#')
    {
        if (!manager.hasChannel(target))
        {
            user.appendOutBuffer(
                r.NumericReply(ERR_NOSUCHCHANNEL,
                               user.getNickname(),
                               target,
                               ""));
            return;
        }

        Channel* channel = manager.findChannel(target);

        if (!channel->isUserInChannel(&user))
        {
            user.appendOutBuffer(
                r.NumericReply(ERR_CANNOTSENDTOCHAN,
                               user.getNickname(),
                               target,
                               ""));
            return;
        }

        std::string msg =
            ":" + user.getNickname() +
            "!" + user.getUsername() +
            "@localhost PRIVMSG " +
            target +
            " :" + message + "\r\n";

        //fix double message
        channel->broadcastExcept(msg, &user);
        return;
    }

    // USER MESSAGE
    User* targetUser = serv.findClientByNick(target);

    std::cout << "PRIVMSG to user [" << target << "] found=" << (targetUser ? "yes" : "no") << std::endl;

    if (!targetUser)
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NOSUCHNICK,
                           user.getNickname(),
                           target,
                           ""));
        return;
    }

    std::string msg =
        ":" + user.getNickname() +
        "!" + user.getUsername() +
        "@localhost PRIVMSG " +
        target +
        " :" + message + "\r\n";

    targetUser->appendOutBuffer(msg);
}
