#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include <iostream>

class Command{

    private:
        std::string 				command;
        std::vector<std::string> 	params;
		//dagouill : added prefix (nick!user@host), trailing (everything after :, the message)
		//and raw (the original line for debugging)
		std::string 				prefix;
		std::string 				trailing;
		std::string 				raw;
	
	public:
		Command();
		~Command();

		bool 						hasPrefix() const;
		bool 						hasTrailing() const;
		bool 						hasParams() const;

		const std::string			&getPrefix() const;
		const std::string			&getCmd() const;
		const std::string			&getTrailing() const;
		std::string 				getParam(size_t i) const;

		std::string 				getTarget() const;
		std::string 				getMessage() const;

		void						ParseCommand(std::string line);
		bool						isValidPrefix(const std::string &prefix);
		bool						isValid() const;

		std::string 				debugString() const;
};

std::string 						trimLeft(const std::string &str);

#endif


//PRIVMSG #chan :hello world how are you
