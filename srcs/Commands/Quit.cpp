#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Quit::~Quit(){}

Quit & Quit::operator=(const Quit & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Quit::Quit(server * serv):Command(serv)
{
	_args[MESSAGE].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Quit::execute()
{
	std::string	*	arg = _args[MESSAGE].arg;
	if (!arg)
	{
		_expeditor->setLogged(false);
		return ;
	}
	_serv->remove_user_from_channels(_expeditor, " QUIT :" + *arg);
	_expeditor->setLogged(false);
	return ;
}
