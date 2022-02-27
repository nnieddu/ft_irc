
#pragma once

#include <map>

#include "User.hpp"

class server;

class Command
{
	protected:
		Command(const Command& x);

		user *			_expeditor;
		std::string *	_channel;
		std::string *	_user;
		std::string *	_arg;
		bool			_chan;
		bool			_usr;
		bool			_argument;
		int				_reply;

	public:

		server *		serv;

		Command();
		Command& operator=(const Command& x);
		Command(server * serv);
		virtual ~Command();

		virtual int	execute();

		void			setExpeditor(user * expeditor);
		virtual void	setArgs(std::vector<std::string> args);

		int				getReply() const;

		bool			needChannel() const;
		bool			needUser() const;
		bool			needArg() const;

		void			reset();
};

class Pass : public Command
{
	public:

	Pass();
	virtual ~Pass();
	Pass&	operator=(const Pass & x);
	Pass(server * serv);
	virtual int	execute();
};

class Nick : public Command
{
	public:

	Nick();
	virtual ~Nick();
	Nick&	operator=(const Nick & x);
	Nick(server * serv);
	virtual int	execute();
};

class User : public Command
{
	public:

	User();
	virtual ~User();
	User&	operator=(const User & x);
	User(server * serv);
	virtual int	execute();
};

class Join : public Command
{
	public:

	Join();
	virtual ~Join();
	Join&	operator=(const Join & x);
	Join(server * serv);
	virtual int	execute();
};

class List : public Command
{
	public:

	List();
	virtual ~List();
	List&	operator=(const List & x);
	List(server * serv);
	virtual int	execute();
};

class Oper : public Command
{
	public:

	Oper();
	virtual ~Oper();
	Oper&	operator=(const Oper & x);
	Oper(server * serv);
	virtual int	execute();
};

class Quit : public Command
{
	public:

	Quit();
	virtual ~Quit();
	Quit&	operator=(const Quit & x);
	Quit(server * serv);
	virtual int	execute();
};

class Mode : public Command
{
	public:

	Mode();
	virtual ~Mode();
	Mode&	operator=(const Mode & x);
	Mode(server * serv);
	virtual int	execute();
};

class Topic : public Command
{
	public:

	Topic();
	virtual ~Topic();
	Topic&	operator=(const Topic & x);
	Topic(server * serv);
	virtual int	execute();
};

class Names : public Command
{
	public:

	Names();
	virtual ~Names();
	Names&	operator=(const Names & x);
	Names(server * serv);
	virtual int	execute();
};

class Invite : public Command
{
	public:

	Invite();
	virtual ~Invite();
	Invite&	operator=(const Invite & x);
	Invite(server * serv);
	virtual int	execute();
};

class Kick : public Command
{
	public:

	Kick();
	virtual ~Kick();
	Kick&	operator=(const Kick & x);
	Kick(server * serv);
	virtual int	execute();
};
