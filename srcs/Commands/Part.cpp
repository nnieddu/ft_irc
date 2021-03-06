#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Part::~Part(){}

Part & Part::operator=(const Part & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Part::Part(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
	_args[MESSAGE].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Part::execute()
{
	std::string	*		channel = _args[CHANNEL].arg;
	std::string	*		arg = _args[MESSAGE].arg;


	if (!channel)
	{
		_serv->send_replies(_expeditor, "PART :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}

	std::deque<std::string> list = _args[CHANNEL].parseList();
	std::string quit;

	while (!list.empty())
	{
		std::deque<std::string>::iterator	it = list.begin();
		
		*it = nameCaseIns(*it);
		if (_serv->channels.find(*it) == _serv->channels.end())
		{
			_serv->send_replies(_expeditor, *it + " :No such channel", ERR_NOSUCHCHANNEL);
			list.pop_front();
			continue ;
		}
		if (!_expeditor->isMember(*it))
		{
			_serv->send_replies(_expeditor, *it + " :You're not on that channel", ERR_NOTONCHANNEL);
			list.pop_front();
			continue ;
		}
		if (arg)
			quit = ":" + _expeditor->getNickname() + " PART " + *it + " " + *arg + "\r\n";
		else
			quit = ":" + _expeditor->getNickname() + " PART " + *it + "\r\n";
		send(_expeditor->getSock(), quit.c_str(), quit.length(), 0);
		if (arg)
			_serv->remove_user_from(_expeditor, *it, " PART " + *it + " " + *arg);
		else
			_serv->remove_user_from(_expeditor, *it, "PART");
		list.pop_front();
	}
}
