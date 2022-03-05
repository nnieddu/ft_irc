#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Info::~Info(){}
Info & Info::operator=(const Info & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

/*----------------------------------------------------------------------------*/

Info::Info(server * serv):Command(serv) {}
void Info::execute()
{
	_serv->send_replies(_expeditor, " : " + _serv->getName() + " running 1.42", RPL_INFO);
	_serv->send_replies(_expeditor, " :End of /INFO list", RPL_ENDOFINFO);
	return ;
}
