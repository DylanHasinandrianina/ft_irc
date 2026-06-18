#include "channelmanager.hpp"
#include "channels.hpp"

ChannelManager::ChannelManager()
{}

ChannelManager::~ChannelManager()
{}

bool ChannelManager::hasChannel(const std::string& name) const
{
    return _channels.find(name) != _channels.end();
}

Channel* ChannelManager::findChannel(const std::string& name)
{
    std::map<std::string, Channel>::iterator it = _channels.find(name);
    if (it == _channels.end())
        return NULL;
    return &it->second;
}

Channel& ChannelManager::createChannel(const std::string& name)
{
	std::pair<std::map<std::string, Channel>::iterator, bool> res =

		_channels.insert(std::make_pair(name, Channel(name)));

	return res.first->second;
}

void ChannelManager::removeChannel(const std::string& name)
{
    _channels.erase(name);
}

const std::map<std::string, Channel>& ChannelManager::getChannels() const
{
    return _channels;
}

std::map<std::string, Channel>& ChannelManager::getChannels()
{
    return _channels;
}

void ChannelManager::removeUserFromAllChannels(User& user)
{
    for (std::map<std::string, Channel>::iterator it = _channels.begin();
         it != _channels.end(); )
    {
        Channel& channel = it->second;

        channel.removeUser(&user);
        channel.removeOperator(&user);

        if (channel.getNbUser() == 0)
            _channels.erase(it++);
        else
            ++it;
    }
}

void ChannelManager::broadcastToUser(User& user, const std::string& msg)
{
    for (std::map<std::string, Channel>::iterator it = _channels.begin();
         it != _channels.end();
         ++it)
    {
        Channel& ch = it->second;

        if (ch.isUserInChannel(&user))
            ch.broadcast(msg);
    }
}
