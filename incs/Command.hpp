#pragma once

#include <map>
#include <deque>

#include "User.hpp"

#define RECEIVER	0
#define NICK		1
#define CHANNEL		2
#define USER		4
#define ARGUMENT	5

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

		int				_reply;

	public:

		server *		serv;

		Command();
		Command(server * serv);
		Command& operator=(const Command& x);
		virtual ~Command();

		virtual int	execute();

		void			setExpeditor(user * expeditor);
		virtual void	setArgs(std::vector<std::string *> args);

		int				getReply() const;

		bool			needReceiver() const;
		bool			needNick() const;
		bool			needChannel() const;
		bool			needUser() const;
		bool			needArg() const;

		void			reset();
};
