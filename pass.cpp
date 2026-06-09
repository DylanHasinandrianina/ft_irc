#include "replybuilder.hpp"
#include "command.hpp"
#include "user.hpp"

class Pass
{
public:
    static void execute(const Command& cmd, User& user, const std::string& serverPassword);
};

void Pass::execute(const Command& cmd, User& user, const std::string& serverPassword)
{
    ReplyBuilder r;

    // already registered
    if (user.isAuthenticated())
    {
        userSend(user,
            r.NumericReply(ERR_ALREADYREGISTERED,
                           user.getNickname(),
                           "",
                           ""));
        return;
    }

    // missing parameter
    if (cmd.paramCount() < 1)
    {
        userSend(user,
            r.NumericReply(ERR_NEEDMOREPARAMS,
                           user.getNickname(),
                           "PASS",
                           ""));
        return;
    }

    const std::string& password = cmd.getParam(0);

    // wrong password
    if (password != serverPassword)
    {
        userSend(user,
            r.NumericReply(ERR_PASSWDMISMATCH,
                           user.getNickname(),
                           "",
                           ""));
        return;
    }

    // success
    user.setPassOk(true);
}
