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
	_args[CHANNEL].isNeeded = true;
	_args[USER].isNeeded = true;
	_args[MESSAGE].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void User::execute()
{
	std::string	*	userName = _args[NICK].arg;
	std::string	*	mode = _args[CHANNEL].arg;
	std::string	*	rlName = _args[MESSAGE].arg;

	if (!userName || !mode || !rlName)
	{
		_serv->send_replies(_expeditor, "USER :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}
	if (_expeditor->getisLogged() == true)
		_serv->send_replies(_expeditor, " :You may not reregister", ERR_ALREADYREGISTRED);
	else if (_expeditor->getisLogged() != true && _expeditor->getLogLvl() == 2)
	{
		// _expeditor->setUsername(*userName);
		_expeditor->setUsername(*userName);
		_expeditor->setRealname(*rlName);
		if (*mode == "8")
			_expeditor->setInvisible();
		else if (*mode == "4")
			_expeditor->setWallOp();
		else if (*mode == "12") {
			_expeditor->setWallOp();
			_expeditor->setInvisible();
		}
		_expeditor->setLogged(true);
		_serv->welcomeNewUser(_expeditor);
	}
}
