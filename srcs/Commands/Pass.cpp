#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Pass::~Pass() {}

Pass & Pass::operator=(const Pass & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Pass::Pass(server * serv):Command(serv)
{
	_args[PASS].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Pass::execute()
{
	std::string	*	pass = _args[PASS].arg;
	if (!pass)
	{
		_serv->send_replies(_expeditor, "PASS :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}
	else if (*pass != _serv->getPassword())
	{
		_serv->send_replies(_expeditor, ":Password incorrect", ERR_PASSWDMISMATCH);
		return ;
	}
	if (!_expeditor->getisLogged() && _expeditor->getLogLvl() == 0)
		_expeditor->setLogLvl(1);
}
