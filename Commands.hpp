
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "User.hpp"
#include "Server.hpp"

//class server;

class Commands
{
	private:
		Commands();
		Commands(const Commands& x);
		Commands& operator=(const Commands& x);

		server *	_serv;

		bool	_is_complete(std::string & cmd) const;

	public:
		Commands(server * serv);
		~Commands();

		typedef void (Commands::*ft_ptr)(user &, std::string);
		ft_ptr ptr;
		std::map<std::string, ft_ptr> cmds_list;

		void launch(user & usr);
		void listCommands();
		std::string parseCmds(std::string cmd);
		std::string parseCmdsArgs(std::string cmd);

		//void exit();
		void join(user & usr, std::string arg);

};
