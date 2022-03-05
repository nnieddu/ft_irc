#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Version::~Version(){}
Version & Version::operator=(const Version & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}
Version::Version(server * serv):Command(serv) {}

/*----------------------------------------------------------------------------*/

void Version::execute()
{
	_serv->send_replies(_expeditor, " : " + _serv->getName() + " running version 1.42", RPL_VERSION);
	return ;
}
