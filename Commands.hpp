
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <functional>

class Commands
{
	private:
		Commands(const Commands& x);
		Commands& operator=(const Commands& x);
	public:
		Commands();
		~Commands();

		typedef void (Commands::*ft_ptr)();
		ft_ptr ptr;
		std::map<std::string, ft_ptr> cmds_list; // remplacer par map de ptr de ft

		void isCommands(std::string cmd);
		void listCommands();

		void exit();
		void join(std::string channel_name);

};
