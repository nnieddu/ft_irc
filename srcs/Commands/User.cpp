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
	// if (_expeditor->getisLogged() != true)
	// {
	// 	std::string prefix = ":" + _expeditor->getUsername();
	// 	std::string logged = prefix +  " " + RPL_WELCOME + " " + _expeditor->getUsername() + \
	// 		" Welcome to the voidernet Relay Network :" + _expeditor->getUsername() + "\r\n";
	// 	send(_expeditor->getSock(), logged.c_str(), logged.length(), 0);
	// 	return ;
	// }
	// if (_expeditor->getisLogged() == true)
	// 	_serv->send_replies(_expeditor, " :You may not reregister", ERR_ALREADYREGISTRED);
	return ;
}
