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
	///// Iterrer dans les args avec nouveau parsing lorsque plusieurs noms de chans en une cmd
	std::string	*	arg = _args[CHANNEL].arg;
	std::cout << "PART ARG =" << *arg << std::endl;
	if (!arg)
	{
		_serv->send_replies(_expeditor, "PART :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}
	*arg = nameCaseIns(*arg);
	if (_serv->channels.find(*arg) == _serv->channels.end())
	{
		_serv->send_replies(_expeditor, *arg + " :No such channel", ERR_NOSUCHCHANNEL);
		return ;
	}
	if (!_expeditor->isMember(*arg))
	{
		_serv->send_replies(_expeditor, *arg + " :You're not on that channel", ERR_NOTONCHANNEL);
		return ;
	}
	std::string quit = ":" + _expeditor->getNickname() + " PART " + *arg + "\r\n";
	send(_expeditor->getSock(), quit.c_str(), quit.length(), 0);
	_serv->remove_user_from(_expeditor, *arg, "PART");
	return ;
}
