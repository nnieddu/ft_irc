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
	_args[NICK].isNeeded = true;
	_args[MESSAGE].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void	Kill::execute()
{
	std::string	*	nick = _args[NICK].arg;
	std::string	*	msg = _args[MESSAGE].arg;

	if (!_expeditor->isServOp())
		return _serv->send_replies(_expeditor, "KILL :You don't haver server operator privileges", ERR_NOPRIVILEGES);
	if (!nick || !msg)
		return _serv->send_replies(_expeditor, "KILL :Not enough parameters", ERR_NEEDMOREPARAMS);
	if (!_serv->isUser(*nick))
		return _serv->send_replies(_expeditor, "KILL :No such nick", ERR_NOSUCHNICK);

	_serv->kill(_expeditor, _serv->getUser(*nick), *msg);
	return ;
}