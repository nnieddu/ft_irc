#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

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

	if (!usr || (!pass && !_serv->getPassword().empty()))
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

/*
4.1.5 Oper

      Command: OPER
   Parameters: <user> <password>

   OPER message is used by a normal user to obtain operator privileges.
   The combination of <user> and <password> are required to gain
   Operator privileges.

   If the client sending the OPER command supplies the correct password
   for the given user, the server then informs the rest of the network
   of the new operator by issuing a "MODE +o" for the clients nickname.

   The OPER message is client-server only.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              RPL_YOUREOPER
           ERR_NOOPERHOST                  ERR_PASSWDMISMATCH

   Example:

   OPER foo bar                    ; Attempt to register as an operator
                                   using a username of "foo" and "bar" as
                                   the password.
*/
