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

		Pass();
		virtual ~Pass();
		Pass&	operator=(const Pass&);
		Pass(server*);
		virtual void execute();
};

class Nick : public Command
{
	public:

		Nick();
		virtual ~Nick();
		Nick&	operator=(const Nick&);
		Nick(server*);
		virtual void execute();
};

class User : public Command
{
	public:

		User();
		virtual ~User();
		User&	operator=(const User&);
		User(server*);
		virtual void execute();
};

class Join : public Command
{
	public:

		Join();
		virtual ~Join();
		Join&	operator=(const Join&);
		Join(server*);
		virtual void execute();
};

class List : public Command
{
	public:

		List();
		virtual ~List();
		List&	operator=(const List&);
		List(server*);
		virtual void execute();
};

class Oper : public Command
{
	public:

		Oper();
		virtual ~Oper();
		Oper&	operator=(const Oper&);
		Oper(server*);
		virtual void execute();
};

class Quit : public Command
{
	public:

		Quit();
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

		Mode();
		Mode(server*);
		virtual ~Mode();
		Mode&	operator=(const Mode&);
		virtual void execute();
};

class Topic : public Command
{
	public:

		Topic();
		virtual ~Topic();
		Topic&	operator=(const Topic&);
		Topic(server*);
		virtual void execute();
};

class Names : public Command
{
	public:

		Names();
		virtual ~Names();
		Names&	operator=(const Names&);
		Names(server*);
		virtual void execute();
};

class Invite : public Command
{
	public:

		Invite();
		virtual ~Invite();
		Invite&	operator=(const Invite&);
		Invite(server*);
		virtual void execute();
};

class Kick : public Command
{
	public:

		Kick();
		virtual ~Kick();
		Kick&	operator=(const Kick&);
		Kick(server*);
		virtual void execute();
};

class Privmsg : public Command
{
	public:

		Privmsg();
		virtual ~Privmsg();
		Privmsg&	operator=(const Privmsg&);
		Privmsg(server*);
		virtual void execute();
};


class Ping : public Command
{
	public:

		Ping();
		virtual ~Ping();
		Ping&	operator=(const Ping&);
		Ping(server*);
		virtual void execute();
};

class Part : public Command
{
	public:

		Part();
		virtual ~Part();
		Part&	operator=(const Part&);
		Part(server*);
		virtual void execute();
};

class Version : public Command
{
	public:

		Version();
		virtual ~Version();
		Version&	operator=(const Version&);
		Version(server*);
		virtual void execute();
};

class Info : public Command
{
	public:

		Info();
		virtual ~Info();
		Info&	operator=(const Info&);
		Info(server*);
		virtual void execute();
};

class Whois : public Command
{
	public:

		Whois();
		virtual ~Whois();
		Whois&	operator=(const Whois&);
		Whois(server*);
		virtual void execute();
};

// class Who : public Command
// {
// 	public:

// 		Who();
// 		virtual ~Who();
// 		Who&	operator=(const Who&);
// 		Who(server*);
// 		virtual void execute();
// };
