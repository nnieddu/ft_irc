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

	if (_serv->send_msg_to_user(_expeditor, _serv->getUser(*nick), *arg, "", false) == -1)
			std::cerr << strerror(errno) << std::endl;

	return ;
}
