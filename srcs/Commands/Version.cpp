#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Version::~Version(){}
Version & Version::operator=(const Version & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}
Version::Version(server * serv):Command(serv) 
{
	_args[HOSTNAME].isNeeded = true;
}
/*----------------------------------------------------------------------------*/

void Version::execute()
{
	std::string	*	arg = _args[HOSTNAME].arg;

	if (!arg || arg->compare(_serv->getName()) == 0 || arg->compare("ircserv") == 0)
	{
		_serv->send_replies(_expeditor, "ircserv(" + _serv->getName()+ ") v1.42 :stable version for correction", RPL_VERSION);
		return ;
	}
	_serv->send_replies(_expeditor, *arg + " No such server", ERR_NOSUCHSERVER);
}
