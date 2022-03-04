
#pragma once

#include <map>
#include <vector>

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

		bool			hasEOC(const std::string * buf) const;
		std::string *	isEOC(std::string * buf) const;

		int launch(user & usr);
		int	cmd_found(Command* cmd, user & usr, std::vector<std::string *>* args);
		int	cmd_not_found(user & usr, std::vector<std::string *>* args) const;
	
		std::vector<std::string *>* parseCmds(std::string * buf) const;

		std::string *	GetNextWord(std::string * buf) const;
		std::string *	parseWord(std::string * buf) const;
		std::string *	parseMessage(std::string * buf) const;

		void	clearLeftover(std::vector<std::string *>* args) const;
};
