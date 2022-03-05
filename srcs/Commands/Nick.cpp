#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Nick::Nick():Command()
{
	_args[NICK].isNeeded = true;
}

Nick::~Nick() {}

Nick & Nick::operator=(const Nick & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Nick::Nick(server * serv):Command(serv)
{
	_args[NICK].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Nick::execute()
{
	std::string	*	arg = _args[NICK].arg;

	if (!arg)
		_serv->send_replies(_expeditor, " :No nickname given", ERR_NONICKNAMEGIVEN);
	if (*arg == "anonymous") //// check char spe/grammar protocol rfc
		_serv->send_replies(_expeditor, " :Erroneus nickname", ERR_ERRONEUSNICKNAME);
	if (_serv->isUser(*arg) == false)
	{
		std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *arg + "\r\n";
	    send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
		_expeditor->setNickname(*arg);
		return ;
	}
	else
		_serv->send_replies(_expeditor, *arg + " :Nickname is already in use", ERR_NICKNAMEINUSE);
	return ;
}
// ERR_NICKCOLLISION osef ?
