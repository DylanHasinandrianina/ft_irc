#include "channels.hpp"

Channel::Channel(std::string name){
    _name = name;

    _userLimit = 0;
    _inviteMode = false;
    _keyRequiredMode = false;
    _topicOpMode = false;
}

Channel::~Channel(){}

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
    _operators[usr->getFd()] = usr;
}

void Channel::removeOperator(User* usr){
    _operators.erase(usr->getFd());
}
