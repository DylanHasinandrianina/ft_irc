
#include "replybuilder.hpp"
#include "dispatch.hpp"
#include "command.hpp"
#include "server.hpp"
#include "user.hpp"
#include "pass.hpp"

Dispatcher::Dispatcher() {}

Dispatcher::~Dispatcher() {}

void Dispatcher::dispatch(const Command& command, User& client, Server& serv)
{
    const std::string& cmd = command.getCmd();
    const std::string& pswd = serv.getPassword();

	std::cout << "[DISPATCH] cmd=" << cmd << std::endl;

    if (cmd == "PASS")
        return Pass::execute(command, client, pswd);

    // if (cmd == "NICK")
    //     return Nick::execute(msg, client);
    //
    // if (cmd == "USER")
    //     return UserCmd::execute(command, client);
    //
    // if (cmd == "PRIVMSG")
    //     return Privcommand::execute(msg, client);
    //
    // if (cmd == "KICK")
    //     return Kick::execute(command, client);
    //
    // if (cmd == "PART")
    //     return Part::execute(command, client);
    //
    // if (cmd == "INVITE")
    //     return Invite::execute(command, client);
    //
    // if (cmd == "TOPIC")
    //     return Topic::execute(command, client);
    //
    // if (cmd == "MODE")
    //     return Mode::execute(command, client);
    //
    // if (cmd == "QUIT")
    //     return Quit::execute(command, client);

}
