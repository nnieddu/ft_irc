#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Who::~Who(){}
Who & Who::operator=(const Who & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Who::Who(server * serv):Command(serv) 
{
	_args[NICK].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Who::execute()
{
	std::string	*	arg = _args[NICK].arg;

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
	_serv->send_replies(_expeditor, *arg + " No such server", ERR_NOSUCHSERVER);
	return ;
}

//       Command: WHO
//    Parameters: [<name> [<o>]]

//    The WHO message is used by a client to generate a query which returns
//    a list of information which 'matches' the <name> parameter given by
//    the client.  In the absence of the <name> parameter, all visible
//    (users who aren't invisible (user mode +i) and who don't have a
//    common channel with the requesting client) are listed.  The same
//    result can be achieved by using a <name> of "0" or any wildcard which
//    will end up matching every entry possible.

//    The <name> passed to WHO is matched against users' host, server, real
//    name and nickname if the channel <name> cannot be found.

//    If the "o" parameter is passed only operators are returned according
//    to the name mask supplied.

//    Numeric Replies:

//            ERR_NOSUCHSERVER
//            RPL_WHOREPLY         RPL_ENDOFWHO