#include "channels.hpp"

Channel::Channel(std::string name){
    _name = name;

    _userLimit = 0;
    _inviteMode = false;
    _keyRequiredMode = false;
    _topicOpMode = false;
}

Channel::~Channel(){}

void Channel::setTopic(const std::string& topic)
{
    _topic = topic;
}

void Channel::addUser(User* usr){
    if (!isUserInChannel(usr))
        _users[usr->getFd()] = usr;
}

void Channel::removeUser(User *usr){
    _users.erase(usr->getFd());
}

bool Channel::isOperator(User* usr){
    if (_operators.find(usr->getFd()) != _operators.end())
        return true;
    return false;
}

bool Channel::isUserInChannel(User* usr){
    if (_users.find(usr->getFd()) != _users.end())
        return true;
    return false;
}

void Channel::addOperator(User* usr){
	if (isUserInChannel(usr))
		_operators[usr->getFd()] = usr;
}

void Channel::removeOperator(User* usr){
    _operators.erase(usr->getFd());
}


size_t Channel::getNbOperators() const
{
    return _operators.size();
}

bool Channel::isEmpty() const
{
    return _users.empty();
}

void Channel::setInviteOnlyMode(bool mode)
{
    _inviteMode = mode;
}

void Channel::setTopicOpMode(bool mode)
{
    _topicOpMode = mode;
}

void Channel::setPassword(const std::string& password)
{
    _password = password;
}

void Channel::setUserLimit(int limit)
{
    if (limit < 0)
        _userLimit = 0;
    else
        _userLimit = limit;
}

void Channel::broadcast(const std::string& msg)
{
    for (std::map<int, User*>::iterator it = _users.begin();
         it != _users.end();
         ++it)
    {
        it->second->appendOutBuffer(msg);
    }
}

void Channel::broadcastExcept(const std::string& msg, User* exclude)
{
    for (std::map<int, User*>::iterator it = _users.begin();
         it != _users.end(); ++it)
    {
        if (it->second->getFd() != exclude->getFd())
            it->second->appendOutBuffer(msg);
    }
}
