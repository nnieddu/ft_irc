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
}

/*----------------------------------------------------------------------------*/

void Part::execute()
{
	std::string	*		channel = _args[CHANNEL].arg;
	std::deque<std::string>	lst;

	if (!channel)
	{
		_serv->send_replies(_expeditor, "PART :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}

	std::deque<std::string> list = _args[CHANNEL].parseList();

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
		std::string quit = ":" + _expeditor->getNickname() + " PART " + *it + "\r\n";
		send(_expeditor->getSock(), quit.c_str(), quit.length(), 0);
		_serv->remove_user_from(_expeditor, *it, "PART");
		list.pop_front();
	}
	// std::string	*	arg = _args[CHANNEL].arg;
	// std::cout << "PART ARG =" << *arg << std::endl;
	// if (!arg)
	// {
	// 	_serv->send_replies(_expeditor, "PART :Not enough parameters", ERR_NEEDMOREPARAMS);
	// 	return ;
	// }
	// *arg = nameCaseIns(*arg);
	// if (_serv->channels.find(*arg) == _serv->channels.end())
	// {
	// 	_serv->send_replies(_expeditor, *arg + " :No such channel", ERR_NOSUCHCHANNEL);
	// 	return ;
	// }
	// if (!_expeditor->isMember(*arg))
	// {
	// 	_serv->send_replies(_expeditor, *arg + " :You're not on that channel", ERR_NOTONCHANNEL);
	// 	return ;
	// }
	// std::string quit = ":" + _expeditor->getNickname() + " PART " + *arg + "\r\n";
	// send(_expeditor->getSock(), quit.c_str(), quit.length(), 0);
	// _serv->remove_user_from(_expeditor, *arg, "PART");
	return ;
}
