#include "user.hpp"

User::User(int fd){
    _fd = fd;
    _passOk = false;
    _nickOk = false;
    _userOk = false;
    _registered = false;
}

User::~User(){}

void User::appendToBuffer(std::string newData){
    _buffer += newData;
    
}

// dagouill : changed function so it removes \n without \t too
bool User::extractCommand(std::string &line){
    size_t pos = _buffer.find("\n");
    if (pos == std::string::npos)
        return false;

    line = _buffer.substr(0, pos);
    _buffer.erase(0, pos + 1);

	if (!line.empty() && line.back() == '\r')
		line.pop_back();

    return true;
}
