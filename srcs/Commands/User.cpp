#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

User::~User(){}

User & User::operator=(const User & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

User::User(server * serv):Command(serv)
{
	_args[NICK].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void User::execute()
{
	std::string	*	arg = _args[NICK].arg;

	if (!arg) 
	{
		_serv->send_replies(_expeditor, "USER :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}
	if (_expeditor->getisLogged() == true)
		_serv->send_replies(_expeditor, " :You may not reregister", ERR_ALREADYREGISTRED);
	else if (_expeditor->getisLogged() != true && _expeditor->getLogLvl() == 2)
	{
		_expeditor->setUsername(*arg);
		_expeditor->setLogged(true);
		_serv->welcomeNewUser(_expeditor);
	}
}
