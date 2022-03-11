#include "../incs/Command.hpp"
#include "../incs/Server.hpp"

#include <deque>

#include <cstdlib>

/*----------------------------------------------------------------------------*/

std::deque<std::string>		Argument::parseList() const
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

/*----------------------------------------------------------------------------*/

Command::Command(server * serv):_serv(serv)
{
	for (size_t elem = 0; elem <= REST; elem++)
	{
		_args[elem].arg = NULL;
		_args[elem].isNeeded = false;
	}
}

Command &	Command::operator=(const Command & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Command::~Command()
{
	for (size_t val = 0; val <= REST; val++)
	{
		if (_args[val].arg)
			delete _args[val].arg;
	}
}

/*----------------------------------------------------------------------------*/

void	Command::setArgs(user * expeditor, std::vector<std::string *>* args)
{
	_expeditor = expeditor;
	delete args->front();
	args->erase(args->begin());

	for (size_t st = 0; st <= REST && cond(args); st++)
	{
		if (_args[st].isNeeded)
		{
			if (st == MESSAGE)
			{
				if (args->front()->front() != ':')
				{
					delete args->front();
					args->front() = NULL;
				}
				else
					args->front()->erase(args->front()->begin());
			}
			_args[st].arg = args->front();
			args->erase(args->begin());
		}
	}
}

void	Command::execute() {}

void	Command::reset()
{
	for (size_t val = 0; val <= REST; val++)
	{
		if (_args[val].arg)
		{
			delete _args[val].arg;
			_args[val].arg = NULL;
		}
	}
}
/*----------------------------------------------------------------------------*/

server*	Command::getServ() const
{
	return _serv;
}

/*----------------------------------------------------------------------------*/

bool	cond(const std::vector<std::string *>* args)
{
	return args->front()->find('\n') == std::string::npos && args->front()->find('\r') == std::string::npos;
}

bool	HasChannelPrefix(const std::string & str)
{
	return (str[0] == '#' || str[0] == '&' || str[0] == '+' || str[0] == '!');
}
