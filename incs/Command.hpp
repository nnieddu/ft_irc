#pragma once

#include <map>

#include "User.hpp"

class server;

class Command
{
	protected:
		Command(const Command& x);

		user *			_expeditor;
		std::string *	_receiver;
		std::string *	_nick;
		std::string *	_channel;
		std::string *	_user;
		std::string *	_arg;
		bool			_rcv;
		bool			_nck;
		bool			_chan;
		bool			_usr;
		bool			_argument;
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
