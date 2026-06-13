#include "replybuilder.hpp"
#include "command.hpp"
#include "user.hpp"
#include "pass.hpp"
#include "server.hpp"

void Pass::execute(const Command& cmd, User& user, Server &serv)
{
    ReplyBuilder r;

	std::cout << "test" << std::endl;
    // already registered
    if (user.isAuthenticated())
    {
        user.appendOutBuffer(
            r.NumericReply(ERR_ALREADYREGISTERED, user.getNickname(), "", ""));
        return;
    }

    // missing parameter
    if (cmd.paramCount() < 1)
    {
        user.appendOutBuffer(
			r.NumericReply(ERR_NEEDMOREPARAMS, user.getNickname(), "PASS", ""));
        return;
    }

    const std::string& password = cmd.getParam(0);

    // wrong password
    if (password != serv.getPassword())
    {
        user.appendOutBuffer(
			r.NumericReply(ERR_PASSWDMISMATCH, user.getNickname(), "", ""));
        return;
    }
    // success
	std::cout << "test2" << std::endl;
    user.setPassOk(true);
	serv.tryRegister(user);
}
