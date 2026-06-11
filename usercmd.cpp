#include "usercmd.hpp"
#include "replybuilder.hpp"
#include "command.hpp"
#include "user.hpp"

void UserCmd::execute(const Command& cmd, User& user)
{
    ReplyBuilder r;

	std::string username = cmd.getParam(0);
	std::string realname = cmd.getTrailing();

	// USER <username> <hostname> <servername> :<realname>
	// so we check trailing to accept old format
	// we accept both
	// USER david 0 * :david Smith
	// USER david 0 * david Smith
	if (realname.empty() && cmd.paramCount() > 3)
		realname = cmd.getParam(3);
    if (username.empty() || realname.empty())
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_NEEDMOREPARAMS,
                           user.getNickname(),
                           "USER",
                           ""));
        return;
    }

    user.setUsername(cmd.getParam(0));

    user.setUserOk(true);
}
