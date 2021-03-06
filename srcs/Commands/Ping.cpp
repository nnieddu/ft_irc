#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Ping::~Ping(){}

Ping & Ping::operator=(const Ping & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Ping::Ping(server * serv):Command(serv)
{
	_args[HOSTNAME].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Ping::execute()
{
	std::string	*	arg = _args[HOSTNAME].arg;

	if (!arg)
	{
		_serv->send_replies(_expeditor, " :No origin specified", ERR_NOORIGIN);
		return ;
	}
	if (*arg != _serv->getHostname() && *arg != _expeditor->getHostname())
		_serv->send_replies(_expeditor, *arg + " :No such server", ERR_NOSUCHSERVER);
	else
	{
		std::string reply = (":" + _expeditor->getUsername() + " PONG " + _serv->getName() + " \r\n");
		send(_expeditor->getSock(), reply.c_str(), reply.length(), 0);
	}
}
