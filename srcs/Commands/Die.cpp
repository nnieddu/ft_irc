#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Die::~Die(){}

Die & Die::operator=(const Die & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Die::Die(server * serv):Command(serv) {}

/*----------------------------------------------------------------------------*/

void	Die::execute()
{
	if (!_expeditor->isServOp())
		return _serv->send_replies(_expeditor, "DIE :You don't haver server operator privileges", ERR_NOPRIVILEGES);

	_serv->_index = -2;
}