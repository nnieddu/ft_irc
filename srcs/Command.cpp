#include "../incs/Command.hpp"
#include "../incs/Server.hpp"

#include <stdlib.h>

Command::Command() {}/*
:
	serv(serv),
	_receiver(NULL),
	_nick(NULL),
	_channel(NULL),
	_user(NULL),
	_arg(NULL),
	_rcv(false),
	_nck(false),
	_chan(false),
	_usr(false),
	_argument(false)
{}*/

Command::Command(server * serv)
:
	serv(serv),
	_receiver(NULL),
	_nick(NULL),
	_channel(NULL),
	_user(NULL),
	_arg(NULL),
	_rcv(false),
	_nck(false),
	_chan(false),
	_usr(false),
	_argument(false)
{}

Command &	Command::operator=(const Command & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Command::~Command()
{
	if (_receiver)
		delete _receiver;
	if (_nick)
		delete _nick;
	if (_channel)
		delete _channel;
	if (_user)
		delete _user;
	if (_arg)
		delete _arg;
}

int	Command::execute() { return 0; }

void	Command::setExpeditor(user * expeditor)
{
	_expeditor = expeditor;
}

void	Command::setArgs(std::vector<std::string *> args)
{
	std::string *	new_ptr;

	while (args.empty() == false)
	{
		new_ptr = args[0];

		if (!_receiver && _rcv)
			_receiver = new_ptr;
		else if (!_nick && _nck)
			_nick = new_ptr;
		else if (!_channel && _chan)
			_channel = new_ptr;
		else if (!_user && _usr)
			_user = new_ptr;
		else if (!_arg && _argument)
			_arg = new_ptr;

		args.erase(args.begin());
	}
}

int	Command::getReply() const{ return _reply; }

bool	Command::needReceiver() const{ return _rcv; }

bool	Command::needNick() const{ return _nck; }

bool	Command::needChannel() const{ return _chan; }

bool	Command::needUser() const{ return _usr; }

bool	Command::needArg() const{ return _argument; }

void	Command::reset()
{
	if (_receiver)
		delete _receiver;
	if (_nick)
		delete _nick;
	if (_channel)
		delete _channel;
	if (_user)
		delete _user;
	if (_arg)
		delete _arg;

	_receiver	= NULL;
	_expeditor	= NULL;
	_nick		= NULL;
	_channel	= NULL;
	_arg		= NULL;
	_channel	= NULL;
}
