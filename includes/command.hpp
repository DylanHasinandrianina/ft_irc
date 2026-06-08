#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include <iostream>

class Command{

    public:
        std::string command;
        std::vector<std::string> params;
};

Command parseCommand(std::string line);
std::string trimLeft(const std::string &str);

#endif


//PRIVMSG #chan :hello world how are you
