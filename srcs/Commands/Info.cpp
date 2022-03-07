#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Info::~Info(){}
Info & Info::operator=(const Info & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

/*----------------------------------------------------------------------------*/

Info::Info(server * serv):Command(serv) {}
void Info::execute()
{
	_serv->send_replies(_expeditor, " :", RPL_INFO);
	_serv->send_replies(_expeditor, " :ircserv (" + _serv->getName() + ") running 1.42", RPL_INFO);
	_serv->send_replies(_expeditor, " :", RPL_INFO);
	_serv->send_replies(_expeditor, " :This server was created and maintained by :", RPL_INFO);
	_serv->send_replies(_expeditor, " :Ludovic Fourage (lfourage)", RPL_INFO);
	_serv->send_replies(_expeditor, " :Nicolas Nieddu (ninieddu) and ", RPL_INFO);
	_serv->send_replies(_expeditor, " :Maximilien Rozniecki (mrozniec)", RPL_INFO);
	_serv->send_replies(_expeditor, " :", RPL_INFO);
	_serv->send_replies(_expeditor, " :Hope you enjoy it !", RPL_INFO);
	_serv->send_replies(_expeditor, " :", RPL_INFO);
	_serv->send_replies(_expeditor, " :", RPL_ENDOFINFO);
	return ;
}
