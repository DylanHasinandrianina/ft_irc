#include "command.hpp"
#include "user.hpp"

class UserCmd
{
public:
    static void execute(const Command& cmd, User& user);
};
