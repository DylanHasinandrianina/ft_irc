#include "command.hpp"
#include "user.hpp"

class Pass
{
public:
    static void execute(const Command& cmd, User& user, const std::string& serverPassword);
};
