#pragma once

#include <map>
#include <deque>

#include "User.hpp"

#define RECEIVER	0
#define NICK		1
#define HOSTNAME	1
#define CHANNEL		2
#define USER		4
#define MESSAGE		5
#define PASS		6

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

		user *					_expeditor;
		std::map<int, Argument>	_args;
		server *				_serv;

	public:
		Command();
		Command(server * serv);
		Command& operator=(const Command&);
		virtual ~Command();

		void			setExpeditor(user*);
		void			setArgs(std::vector<std::string*>*);
		virtual void	execute();
		void			reset();

		server *		getServ() const;

};

bool	HasChannelPrefix(const std::string&);
bool	cond(const std::vector<std::string*>*);
