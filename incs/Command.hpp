#pragma once

#include <map>
#include <deque>

#include "User.hpp"

#define RECEIVER	0
#define NICK		1
#define HOSTNAME	1
#define CHANNEL		2
#define USER		4
#define LIST		5
#define MESSAGE		6
#define PASS		7

class server;

struct	Argument
{
	std::string *				arg;
	bool						isNeeded;
	std::deque<std::string>		parseList();
};

class Command
{
	protected:
		Command(const Command& x);
		Command();

		user *					_expeditor;
		std::map<int, Argument>	_args;
		server *				_serv;

	public:
		Command(server * serv);
		Command& operator=(const Command&);
		virtual ~Command();

		void			setArgs(user *, std::vector<std::string*>*);
		virtual void	execute();
		void			reset();

		server *		getServ() const;

};

bool	HasChannelPrefix(const std::string&);
bool	cond(const std::vector<std::string*>*);
