
#pragma once

#include <map>

#include "User.hpp"
#include "Command.hpp"

class server;

class Interpret
{
	private:
		Interpret();
		Interpret(const Interpret& x);
		Interpret& operator=(const Interpret& x);

		server *	_serv;

	public:
		Interpret(server * serv);
		~Interpret();

		typedef int (Interpret::*ft_ptr)(user *, std::string);
		ft_ptr ptr;

		std::map<std::string, Command *> cmds_list;

		int launch(user & usr);
		std::string parseCmds(std::string * buf);

		std::string	parseChannel(std::string * buf);
		std::string	parseNick(std::string * buf);
		std::string	parseArg(std::string * buf);
};
