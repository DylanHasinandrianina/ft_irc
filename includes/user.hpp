#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <set>

class User{

    private:
        //ip??
        int _fd;
        std::string _nickname;
        std::string _username;

		//server to client
        std::string _outBuffer;
		// client to server
        std::string _InBuffer;

        bool _passOk;
        bool _nickOk;
        bool _userOk;
        bool _registered;
		bool _isDisconnected;

		std::set<std::string> _invitedChannels;

    public:
        User(int fd);
        ~User();

		// INPUT
        void appendToBuffer(std::string newData);
        bool extractCommand(std::string &cmd);

		//OUTPUT
	    void appendOutBuffer(const std::string& msg);
		bool hasOutBuffer() const;
		const std::string& getOutBuffer() const;
		void clearOutBuffer();

        int getFd(){return _fd;}
        const std::string& getUsername() const {return _username;}
        const std::string& getNickname() const {return _nickname;}
        bool isPassOk(){return _passOk;}
        bool isNickOk(){return _nickOk;}
        bool isUserOk(){return _userOk;}
        bool isAuthenticated(){return _registered;}
		bool isDisconnected() {return _isDisconnected;}

        void setNickname(std::string n){_nickname = n;}
        void setUsername(std::string u){_username = u;}
        void setPassOk(bool b){_passOk = b;}
        void setNickOk(bool b){_nickOk = b;}
        void setUserOk(bool b){_userOk = b;}
        void setAuthenticatedOk(bool b){_registered = b;}
		void setDisconnected(bool b) {_isDisconnected = b;}

		void addInvite(const std::string &channel);
		bool isInvited(const std::string &channel)const;
		void removeInvite(const std::string& channel);

};

#endif
