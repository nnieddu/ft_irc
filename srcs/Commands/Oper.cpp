#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Oper::Oper():Command()
{
	_args[USER].isNeeded = true;
	_args[PASS].isNeeded = true;
}

Oper::~Oper(){}

Oper & Oper::operator=(const Oper & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Oper::Oper(server * serv):Command(serv)
{
	_args[USER].isNeeded = true;
	_args[PASS].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Oper::execute()
{
	std::string	*	usr = _args[USER].arg;
	std::string	*	pass = _args[PASS].arg;

	if (!usr || (!pass && !_serv->getPassword().empty()));
		return _serv->send_replies(_expeditor, "OPER :Not enough parameters", ERR_NEEDMOREPARAMS);
	if (!(_serv->isUser(*usr)))
		return _serv->send_replies(_expeditor, "OPER :No operator host", ERR_NOOPERHOST);
	if (pass && _serv->getPassword().compare(*pass))
		return _serv->send_replies(_expeditor, "OPER :Wrong password", ERR_PASSWDMISMATCH);

	if (_serv->getUser(*usr)->isServOp())
		return _serv->send_replies(_expeditor, "", RPL_YOUREOPER);

	_serv->getUser(*usr)->promoteServ();
	_serv->send_replies(_expeditor, "You are now an operator", RPL_YOUREOPER);
}
