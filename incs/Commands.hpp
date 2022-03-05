#pragma once

#include "Command.hpp"


class Pass : public Command
{
	public:

		Pass();
		virtual ~Pass();
		Pass&	operator=(const Pass & x);
		Pass(server*);
		virtual void execute();
};

class Nick : public Command
{
	public:

		Nick();
		virtual ~Nick();
		Nick&	operator=(const Nick & x);
		Nick(server*);
		virtual void execute();
};

class User : public Command
{
	public:

		User();
		virtual ~User();
		User&	operator=(const User & x);
		User(server*);
		virtual void execute();
};

class Join : public Command
{
	public:

		Join();
		virtual ~Join();
		Join&	operator=(const Join & x);
		Join(server*);
		virtual void execute();
};

class List : public Command
{
	public:

		List();
		virtual ~List();
		List&	operator=(const List & x);
		List(server*);
		virtual void execute();
};

class Oper : public Command
{
	public:

		Oper();
		virtual ~Oper();
		Oper&	operator=(const Oper & x);
		Oper(server*);
		virtual void execute();
};

class Quit : public Command
{
	public:

		Quit();
		virtual ~Quit();
		Quit&	operator=(const Quit & x);
		Quit(server*);
		virtual void execute();
};

class Mode : public Command
{
	public:

		Mode();
		virtual ~Mode();
		Mode&	operator=(const Mode & x);
		Mode(server*);
		virtual void execute();
};

class Topic : public Command
{
	public:

		Topic();
		virtual ~Topic();
		Topic&	operator=(const Topic & x);
		Topic(server*);
		virtual void execute();
};

class Names : public Command
{
	public:

		Names();
		virtual ~Names();
		Names&	operator=(const Names & x);
		Names(server*);
		virtual void execute();
};

class Invite : public Command
{
	public:

		Invite();
		virtual ~Invite();
		Invite&	operator=(const Invite & x);
		Invite(server*);
		virtual void execute();
};

class Kick : public Command
{
	public:

		Kick();
		virtual ~Kick();
		Kick&	operator=(const Kick & x);
		Kick(server*);
		virtual void execute();
};

class Privmsg : public Command
{
	public:

		Privmsg();
		virtual ~Privmsg();
		Privmsg&	operator=(const Privmsg & x);
		Privmsg(server*);
		virtual void execute();
};


class Ping : public Command
{
	public:

		Ping();
		virtual ~Ping();
		Ping&	operator=(const Ping & x);
		Ping(server*);
		virtual void execute();
};

class Part : public Command
{
	public:

		Part();
		virtual ~Part();
		Part&	operator=(const Part & x);
		Part(server*);
		virtual void execute();
};

class Version : public Command
{
	public:

		Version();
		virtual ~Version();
		Version&	operator=(const Version & x);
		Version(server*);
		virtual void execute();
};

class Info : public Command
{
	public:

		Info();
		virtual ~Info();
		Info&	operator=(const Info & x);
		Info(server*);
		virtual void execute();
};

class Whois : public Command
{
	public:

		Whois();
		virtual ~Whois();
		Whois&	operator=(const Whois & x);
		Whois(server*);
		virtual void execute();
};

// class Who : public Command
// {
// 	public:

// 		Who();
// 		virtual ~Who();
// 		Who&	operator=(const Who & x);
// 		Who(server*);
// 		virtual void execute();
// };
