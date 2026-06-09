#include "command.hpp"

Command::Command() {}
Command::~Command() {}

bool Command::hasPrefix() const
{ 
	return !prefix.empty();
}

bool Command::hasTrailing() const
{
	return !trailing.empty();
}

bool Command::hasParams() const
{
	return !params.empty();
}

const std::string& Command::getPrefix() const
{
    return prefix;
}

const std::string& Command::getCmd() const
{
    return command;
}

const std::string& Command::getTrailing() const
{
    return trailing;
}

std::string Command::getParam(size_t i) const
{
    if (i >= params.size())
        return "";
    return params[i];
}

std::string Command::getTarget() const
{
    if (!params.empty())
        return params[0];
    return "";
}

std::string Command::getMessage() const
{
    if (!trailing.empty())
        return trailing;
    if (params.size() > 1)
        return params[1];
    return "";
}

bool isValidPrefix(const std::string& prefix)
{
    if (prefix.empty())
        return false;

    for (size_t i = 0; i < prefix.size(); ++i)
    {
        unsigned char c = prefix[i];

        if (std::isspace(c))
            return false;
    }

    return true;
}

bool Command::isValid() const
{
	return !command.empty();
}

std::string trimLeft(const std::string &str)
{
    size_t i = 0;

    while (i < str.size() && std::isspace(str[i]))
        i++;

    return str.substr(i);
}

std::string Command::debugString() const
{
    std::string out = command;

    for (size_t i = 0; i < params.size(); i++)
        out += " " + params[i];

    if (!trailing.empty())
        out += " :" + trailing;

    return out;
}

std::string toupperStr(std::string str)
{
    std::string up;

    for (size_t i = 0; i < str.size(); i++){
        up += std::toupper(static_cast<unsigned char>(str[i]));
    }

    return up;
}

void	Command::ParseCommand(std::string line)
{
    std::string trimLine = trimLeft(line);

    // =================
    // PREFIX EXTRACTION
    // =================
    if (!trimLine.empty() && trimLine[0] == ':')
    {
        size_t space = trimLine.find(' ');
        if (space == std::string::npos)
            return;

        prefix = trimLine.substr(1, space - 1);

		// we reject malformed prefix, error is silent
		if (!isValidPrefix(prefix))
			return ;
		// trim eventual multiple whitespaces after prefix
		size_t i = space;
		while (i < trimLine.size() && std::isspace(trimLine[i]))
			i++;

        trimLine = trimLine.substr(i);
    }

    // =======
    // COMMAND
    // =======
    size_t i = 0;
	//
    while (i < trimLine.length() && !std::isspace(trimLine[i]))
        i++;

    command = toupperStr(trimLine.substr(0, i));
	if (command.empty())
		return ;
	
	// ============================
    // PARAMS + TRAILING EXTRACTION
    // ============================
    while (i < trimLine.size())
    {

        while (i < trimLine.size() && std::isspace(trimLine[i]))
			i++;

        if (i >= trimLine.size())
            break;
        
        if (trimLine[i] == ':' && params.size() >= 0)
		{
            trailing = trimLine.substr(i + 1);
            break;
        }

		size_t start = i;

        while (i < trimLine.length() && !std::isspace(trimLine[i]))
            i++;

        params.push_back(trimLine.substr(start, i - start));
    }
	raw = line;
}
