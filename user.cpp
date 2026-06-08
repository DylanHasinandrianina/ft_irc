#include "./includes/user.hpp"

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

bool User::extractCommand(std::string &cmd){
    size_t pos = _buffer.find("\r\n");
    if (pos == std::string::npos)
        return false;
    cmd = _buffer.substr(0, pos);
    _buffer.erase(0, pos + 2);
    return true;
}

