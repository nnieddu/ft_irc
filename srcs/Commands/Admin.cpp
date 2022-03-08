#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Admin::~Admin(){}
Admin & Admin::operator=(const Admin & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

/*----------------------------------------------------------------------------*/

Admin::Admin(server * serv):Command(serv) 
{
	_args[HOSTNAME].isNeeded = true;
}

void Admin::execute()
{
	std::string	*	arg = _args[HOSTNAME].arg;

	if (!arg || arg->compare(_serv->getName()) == 0 || arg->compare("ircserv") == 0)
	{
		if (!_serv->getIRCOp().empty())
		{
			_serv->send_replies(_expeditor, _serv->getName() + " :Administrative info", RPL_ADMINME);
			_serv->send_replies(_expeditor, ":The IRC Operator of this server is " + _serv->getIRCOp(), RPL_ADMINLOC1);
			return ;
		}
		_serv->send_replies(_expeditor, _serv->getName() + ":No administrative info available", ERR_NOADMININFO);
		return ;
	}
	_serv->send_replies(_expeditor, *arg + " No such server", ERR_NOSUCHSERVER);
}
