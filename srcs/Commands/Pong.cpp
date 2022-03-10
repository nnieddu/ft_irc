#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Pong::~Pong(){}

Pong & Pong::operator=(const Pong & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Pong::Pong(server * serv):Command(serv)
{
	_args[USER].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Pong::execute()
{
	std::string	*	usr = _args[USER].arg;

	if (!usr)
	{
		_serv->send_replies(_expeditor, " :No origin specified", ERR_NOORIGIN);
		return ;
	}
	usr->erase(usr->begin());
	return _serv->pong(*usr);
}
