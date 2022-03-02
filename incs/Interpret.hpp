
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
		bool		_iseoc;

	public:
		Interpret(server * serv);
		~Interpret();

		typedef int (Interpret::*ft_ptr)(user *, std::string);
		ft_ptr ptr;

		std::map<std::string, Command *> cmds_list;

		int launch(user & usr);
	
		std::string parseCmds(std::string * buf);
		int	cmd_not_found(user & usr);

		std::string *	parseWord(std::string * buf);
		std::string *	parseMessage(std::string * buf);

		std::string *	GetNextWord(std::string * buf);
		std::string *	GetMessageStart(std::string * buf);
		void			clearLeftover(std::string * buf);
};
