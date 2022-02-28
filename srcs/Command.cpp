#include "../incs/Command.hpp"
#include "../incs/Server.hpp"

#include <stdlib.h>

Command::Command() {}//: serv(NULL), _channel(NULL), _user(NULL), _arg(NULL), _chan(false), _usr(false), _argument(false){}

Command &	Command::operator=(const Command & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Command::~Command()
{
	_user = NULL;
	if (_channel)
		delete _channel;
	if (_arg)
		delete _arg;
}

Command::Command(server * serv): serv(serv), _channel(NULL), _user(NULL), _arg(NULL), _nck(false), _chan(false), _usr(false), _argument(false)
{}

int	Command::execute() { return 0; }

void	Command::setExpeditor(user * expeditor)
{
	_expeditor = expeditor;
}

void	Command::setArgs(std::vector<std::string> args)
{
	std::string *	new_ptr;

	while (args.empty() == false)
	{
		new_ptr = new std::string(args[0]);

		if (_nck)
			_nick = new_ptr;
		else if (_chan)
			_channel = new_ptr;
		else if (_usr)
			_user = new_ptr;
		else if (_argument)
			_arg = new_ptr;
		args.erase(args.begin());
		new_ptr = NULL;
	}
}

int	Command::getReply() const{ return _reply; }

bool	Command::needNick() const{ return _nck; }

bool	Command::needChannel() const{ return _chan; }

bool	Command::needUser() const{ return _usr; }

bool	Command::needArg() const{ return _argument; }

void	Command::reset()
{
	_user = NULL;
	if (_channel)
		delete _channel;
	_channel = NULL;
	if (_arg)
		delete _arg;
	_arg = NULL;
}
