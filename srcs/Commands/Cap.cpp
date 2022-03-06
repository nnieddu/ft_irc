#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Cap::~Cap(){}

Cap & Cap::operator=(const Cap & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Cap::Cap(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Cap::execute()
{
	std::string	*	arg = _args[CHANNEL].arg;
	std::string		replie("CAP * LS\n\r");

	if (arg && arg->compare("LS"))
		_serv->send_replies(_expeditor, replie, "");
	return ;
}