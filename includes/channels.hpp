#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "user.hpp"
#include <map>

class Channel{

    private:
        std::string _name;
        std::string _topic;
        std::string _password;

        std::map<int, User*> _users;
        std::map<int, User*> _operators;
        //invitedqueue

        int _userLimit;

        bool _inviteMode;
        bool _keyRequiredMode;
        bool _topicOpMode;

    
    public:
        Channel(std::string name);
        ~Channel();

        const std::string& 			getName() const {return _name;}
        const std::string& 			getTopic() const {return _topic;}
        const std::string& 			getPassword() const {return _password;}
        const std::map<int, User*>& getUsers() const {return _users;}
        const std::map<int, User*>& getOperators() const {return _operators;}
        size_t 						getNbUser() const {return _users.size();}
		size_t						getNbOperators() const;

		void 						setTopic(const std::string& topic);

        void 						addUser(User*);
        void 						removeUser(User*);
        void 						addOperator(User*);
        void 						removeOperator(User*);


        bool 						isOperator(User*);
        bool 						isUserInChannel(User*);
		bool 						isEmpty() const;

        int 						getUserLimit() {return _userLimit;}
        bool 						isInviteOnlyMode() {return _inviteMode;}
        bool 						isKeyRequiredMode() {return _keyRequiredMode;}
        bool 						isTopicOpMode() {return _topicOpMode;}

		void 						setInviteOnlyMode(bool);
		void 						setTopicOpMode(bool);
		void 						setPassword(const std::string&);
		void 						setUserLimit(int);

		void 						broadcast(const std::string& msg);

};

#endif
