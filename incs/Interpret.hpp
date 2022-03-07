
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

		server *							_serv;
		std::map<std::string, Command *>	_cmds_list;

	public:
		Interpret(server * serv);
		virtual ~Interpret();

		void 						treat_user_buffer(user&);
		std::vector<std::string *>* parseCmds(std::string*) const;
		std::string *				GetNextWord(std::string*) const;
		std::string *				parseWord(std::string*) const;
		std::string *				parseMessage(std::string*) const;

		void	cmd_found(Command*, user&, std::vector<std::string*>*);
		void	cmd_not_found(user&, std::vector<std::string*>*) const;

		bool			hasEOC(const std::string*) const;
		std::string *	isEOC(std::string*) const;
		void			clearLeftover(std::vector<std::string*>*) const;
};
