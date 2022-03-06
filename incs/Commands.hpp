#pragma once

#include "Command.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "Channel.hpp"
#include "NumericReplies.hpp"

#include <cstdlib>

class Pass : public Command
{
	public:
		virtual ~Pass();
		Pass&	operator=(const Pass&);
		Pass(server*);
		virtual void execute();
};

class Cap : public Command
{
	public:
		virtual ~Cap();
		Cap&	operator=(const Cap&);
		Cap(server*);
		virtual void execute();
};

class Nick : public Command
{
	public:
		virtual ~Nick();
		Nick&	operator=(const Nick&);
		Nick(server*);
		virtual void execute();
};

class User : public Command
{
	public:
		virtual ~User();
		User&	operator=(const User&);
		User(server*);
		virtual void execute();
};

class Join : public Command
{
	public:
		virtual ~Join();
		Join&	operator=(const Join&);
		Join(server*);
		virtual void execute();
};

class List : public Command
{
	public:
		virtual ~List();
		List&	operator=(const List&);
		List(server*);
		virtual void execute();
};

class Oper : public Command
{
	public:
		virtual ~Oper();
		Oper&	operator=(const Oper&);
		Oper(server*);
		virtual void execute();
};

class Quit : public Command
{
	public:
		virtual ~Quit();
		Quit&	operator=(const Quit&);
		Quit(server*);
		virtual void execute();
};

class Mode : public Command {

	private:
		void	modeChan(Channel&, std::string&, std::string&);

		void	modeUser(user&, std::string&, std::string&);

	public:
		Mode(server*);
		virtual ~Mode();
		Mode&	operator=(const Mode&);
		virtual void execute();
};

class Topic : public Command
{
	public:
		virtual ~Topic();
		Topic&	operator=(const Topic&);
		Topic(server*);
		virtual void execute();
};

class Names : public Command
{
	public:
		virtual ~Names();
		Names&	operator=(const Names&);
		Names(server*);
		virtual void execute();
};

class Invite : public Command
{
	public:
		virtual ~Invite();
		Invite&	operator=(const Invite&);
		Invite(server*);
		virtual void execute();
};

class Kick : public Command
{
	public:
		virtual ~Kick();
		Kick&	operator=(const Kick&);
		Kick(server*);
		virtual void execute();
};

class Privmsg : public Command
{
	public:
		virtual ~Privmsg();
		Privmsg&	operator=(const Privmsg&);
		Privmsg(server*);
		virtual void execute();
};

class Ping : public Command
{
	public:
		virtual ~Ping();
		Ping&	operator=(const Ping&);
		Ping(server*);
		virtual void execute();
};

class Pong : public Command
{
	public:
		virtual ~Pong();
		Pong&	operator=(const Pong&);
		Pong(server*);
		virtual void execute();
};

class Part : public Command
{
	public:
		virtual ~Part();
		Part&	operator=(const Part&);
		Part(server*);
		virtual void execute();
};

class Version : public Command
{
	public:
		virtual ~Version();
		Version&	operator=(const Version&);
		Version(server*);
		virtual void execute();
};

class Info : public Command
{
	public:
		virtual ~Info();
		Info&	operator=(const Info&);
		Info(server*);
		virtual void execute();
};

class Whois : public Command
{
	public:
		virtual ~Whois();
		Whois&	operator=(const Whois&);
		Whois(server*);
		virtual void execute();
};

// class Who : public Command
// {
// 	public:
// 		virtual ~Who();
// 		Who&	operator=(const Who&);
// 		Who(server*);
// 		virtual void execute();
// };
