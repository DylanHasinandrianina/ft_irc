#include "./includes/command.hpp"

std::string trimLeft(const std::string &str)
{
    size_t i = 0;

    while (i < str.size() && std::isspace(str[i]))
        i++;

    return str.substr(i);
}

std::string toupperStr(std::string str)
{
    std::string up;

    for (size_t i = 0; i < str.size(); i++){
        up += std::toupper(static_cast<unsigned char>(str[i]));
    }

    return up;
}

Command parseCommand(std::string line)
{
    Command cmd;

    std::string trimLine = trimLeft(line);

    size_t i = 0;
    while (i < trimLine.length() && trimLine[i] != ' ')
        i++;
    std::string lowerCommand= trimLine.substr(0, i);
    cmd.command = toupperStr(lowerCommand);
    
    while (i < trimLine.length())
    {
        while (i < trimLine.length() && trimLine[i] == ' ')
			i++;
        if (i >= trimLine.length())
            break;
        
        if (i < trimLine.length() && trimLine[i] == ':'){
            i++;
            cmd.params.push_back(trimLine.substr(i));
            break;
        }

        std::string param;
        while (i < trimLine.length() && trimLine[i] != ' ')
        {
            param += trimLine[i];
            i++;
        }
        cmd.params.push_back(param);
    }
    return cmd;
}