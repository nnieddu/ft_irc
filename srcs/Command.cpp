#include "../incs/Command.hpp"
#include "../incs/Server.hpp"

#include <map>
#include <deque>

#include <cstdlib>

std::deque<std::string>		Argument::parseList()
{
	std::deque<std::string>	lst;
	size_t					index_bis = 0;

	for(size_t index = 0; index < arg->size(); index++)
	{
		if ((*arg)[index] == ',')
		{
			lst.push_back(std::string().assign(arg->begin() + index_bis, arg->begin() + index));
			index_bis = index + 1;
		}
	}
	lst.push_back(std::string().assign(arg->begin() + index_bis, arg->end()));
	return lst;
}

Command::Command() {}

Command::Command(server * serv):serv(serv)
{
	for (size_t elem = 0; elem <= PASS; elem++)
	{
		_args[elem].arg = NULL;
		_args[elem].isNeeded = false;
	}
}

Command &	Command::operator=(const Command & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Command::~Command()
{
	for (size_t val = 0; val <= PASS; val++)
	{
		if (_args[val].arg)
			delete _args[val].arg;
	}
}

int	Command::execute() { return 0; }

void	Command::setExpeditor(user * expeditor)
{
	_expeditor = expeditor;
}

void	Command::setArgs(std::vector<std::string *> args)
{
	for (size_t val = 0; !(args.empty()); val++)
	{
		if (_args[val].isNeeded)
		{
			_args[val].arg = args[0];
			args.erase(args.begin());
		}
	}
}

int	Command::getReply() const{ return _reply; }

bool	Command::needReceiver() const { return _args.find(RECEIVER)->second.isNeeded; }

bool	Command::needNick() const { return _args.find(NICK)->second.isNeeded; }

bool	Command::needChannel() const { return _args.find(CHANNEL)->second.isNeeded; }

bool	Command::needUser() const { return _args.find(USER)->second.isNeeded; }

bool	Command::needArg() const { return _args.find(MESSAGE)->second.isNeeded; }

bool	Command::needPass() const { return _args.find(PASS)->second.isNeeded; }

bool	Command::isChannelName(std::string & str) { return (str[0] == '#' || str[0] == '&' || str[0] == '+' || str[0] == '!'); }

void	Command::reset()
{
	for (size_t val = 0; val <= PASS; val++)
	{
		if (_args[val].arg)
		{
			delete _args[val].arg;
			_args[val].arg = NULL;
		}
	}
}
