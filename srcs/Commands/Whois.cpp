#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Whois::Whois():Command() 
{
	_args[NICK].isNeeded = true;
}
Whois::~Whois(){}
Whois & Whois::operator=(const Whois & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Whois::Whois(server * serv):Command(serv) 
{
	_args[NICK].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Whois::execute()
{
	std::string	*	arg = _args[NICK].arg;

// RPL_WHOISUSER                   RPL_WHOISCHANNELS
// RPL_WHOISCHANNELS               RPL_WHOISSERVER
// RPL_AWAY                        RPL_WHOISOPERATOR
// RPL_WHOISIDLE                   ERR_NOSUCHNICK
// RPL_ENDOFWHOIS

	if (!arg)
	{
		_serv->send_replies(_expeditor, " :No nickname given", ERR_NONICKNAMEGIVEN);
		return ;
	}
	if (_serv->isUser(*arg) != false)
	{
		std::string str = ": " + _expeditor->getNickname() + " " + _expeditor->getUsername() + 
			+ " " + _expeditor->getHostname() + "\r\n";
	    send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
		return ;
	}
	return ;
}
