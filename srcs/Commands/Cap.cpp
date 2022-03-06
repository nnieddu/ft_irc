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

	if (arg && arg->compare("LS"))
		send(_expeditor->getSock(), "CAP * LS", 8, 0);
	return ;
}