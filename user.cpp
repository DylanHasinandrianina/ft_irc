#include "user.hpp"

User::User(int fd){
    _fd = fd;
    _passOk = false;
    _nickOk = false;
    _userOk = false;
    _registered = false;
	_isDisconnected = false;
}

User::~User(){}

void User::appendToBuffer(std::string newData){
    _InBuffer += newData;
    
}

// append message to outgoing buffer
void User::appendOutBuffer(const std::string& msg)
{
    _outBuffer += msg;
}

// check if there is something to send
bool User::hasOutBuffer() const
{
    return !_outBuffer.empty();
}

// get full outgoing buffer
const std::string& User::getOutBuffer() const
{
    return _outBuffer;
}

// clear buffer (only if fully sent or reset on disconnect)
void User::clearOutBuffer()
{
    _outBuffer.clear();
}

// dagouill : changed function so it removes \n without \t too
bool User::extractCommand(std::string &line){
    size_t pos = _InBuffer.find('\n');
    if (pos == std::string::npos)
        return false;

    line = _InBuffer.substr(0, pos);
    _InBuffer.erase(0, pos + 1);

	if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);

    return true;
}

void User::addInvite(const std::string& channel)
{
    _invitedChannels.insert(channel);
}

bool User::isInvited(const std::string& channel) const
{
    return _invitedChannels.find(channel) != _invitedChannels.end();
}

void User::removeInvite(const std::string& channel)
{
    _invitedChannels.erase(channel);
}
