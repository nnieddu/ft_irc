
#pragma once

#include <map>

#include "User.hpp"

class server;

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

		typedef int (Commands::*ft_ptr)(user *, std::string);
		ft_ptr ptr;

		std::map<std::string, ft_ptr> cmds_list;

		int launch(user & usr);
		std::string parseCmds(std::string cmd);
		std::string parseCmdArgs(std::string arg);

		int pass(user * usr, std::string arg);
		int nick(user * usr, std::string arg);
		int user_cmd(user * usr, std::string arg);
		// void oper(user * usr, std::string arg);
		// void quit(user * usr, std::string arg);
		int join(user * usr, std::string arg);
		// void part(user * usr, std::string arg);
		// void mode(user * usr, std::string arg);
		// void topic(user * usr, std::string arg);
		// void names(user * usr, std::string arg);
		int list(user * usr, std::string arg);
		// void invite(user * usr, std::string arg);
		// void kick(user * usr, std::string arg);
};