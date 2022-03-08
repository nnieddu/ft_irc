#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Kill::~Kill(){}

Kill & Kill::operator=(const Kill & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Kill::Kill(server * serv):Command(serv)
{
	_args[USER].isNeeded = true;
	_args[MESSAGE].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void	Kill::execute()
{
	std::string	*	msg = _args[MESSAGE].arg;

	if (!_args[CHANNEL].arg || !_args[USER].arg)
		return _serv->send_replies(_expeditor, "Kill :Not enough parameters", ERR_NEEDMOREPARAMS);
	
	std::deque<std::string>	user_list = _args[USER].parseList();

}

