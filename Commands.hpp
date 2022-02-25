
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

		typedef void (Commands::*ft_ptr)(std::string);
		ft_ptr ptr;
		std::map<std::string, ft_ptr> cmds_list;

		void launch(std::string full_cmd);
		void listCommands();
		std::string parseCmds(std::string cmd);
		std::string parseCmdsArgs(std::string cmd);

		void exit();
		void join(std::string cmd);

};
