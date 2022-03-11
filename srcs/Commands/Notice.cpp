#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Notice::~Notice() {}

Notice & Notice::operator=(const Notice & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Notice::Notice(server * serv):Command(serv)
{
	_args[NICK].isNeeded	= true;
	_args[MESSAGE].isNeeded	= true;
}

/*----------------------------------------------------------------------------*/

void Notice::execute()
{
	std::string	*			nick = _args[NICK].arg;
	std::string	*			arg = _args[MESSAGE].arg;

	if (!nick || !arg)
		return ;

	std::string fmsg;

	fmsg = ":" + _expeditor->getNickname() + "(" + _expeditor->getUsername() + ") NOTICE " + _serv->getUser(*nick)->getNickname() + " " + *arg + "\r\n";
	send(_serv->getUser(*nick)->getSock(), fmsg.c_str(), fmsg.size(), 0);
}
