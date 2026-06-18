#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include <map>
#include <string>

class Channel;
class User;

class ChannelManager
{
private:
    std::map<std::string, Channel> _channels;

public:
    ChannelManager();
    ~ChannelManager();

    bool        							hasChannel(const std::string& name) const;
    Channel*    							findChannel(const std::string& name);

    Channel&    							createChannel(const std::string& name);
    void        							removeChannel(const std::string& name);

    const std::map<std::string, Channel>& 	getChannels() const;
    std::map<std::string, Channel>& 		getChannels();

	void 									removeUserFromAllChannels(User& user);
	void									broadcastToUser(User& user, const std::string& msg);
};

#endif
