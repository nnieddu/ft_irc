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
	_args[CHANNEL].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Who::execute()
{
	std::map<std::string, unsigned int>	chanlist = _expeditor->getChannels();
	std::vector<user*>					userlist = _serv->getUsers();
	std::string	*						name = _args[NICK].arg;
	user *								target = _serv->getUser(*name);

	if (!name || !name->compare("0"))
	{
		for (std::vector<user*>::iterator it(userlist.begin()); it != userlist.end(); it++)
		{
			if ((*it)->isInvisible())
				break;

			std::map<std::string, unsigned int>::iterator chan_it(chanlist.begin());

			for (; chan_it != chanlist.end(); chan_it++)
			{
				if ((*it)->isMember(chan_it->first))
					break;
			}
			if (chan_it == chanlist.end())
				(*it)->send_who_reply(_expeditor, NULL);
		}
		if (!name)
			return _serv->send_replies(_expeditor, " * :End of who list", RPL_ENDOFWHO);
		return _serv->send_replies(_expeditor, " 0 :End of who list", RPL_ENDOFWHO);

	}
	else if (_serv->isUser(*name) && !target->isInvisible())
	{
		std::map<std::string, unsigned int>::iterator	chan_it(chanlist.begin());

		for (; chan_it != chanlist.end(); chan_it++)
		{
			if (target->isMember(chan_it->first))
				break;
		}

		if (chan_it != chanlist.end())
		{
			target->send_who_reply(_expeditor, _serv->getChannel(chan_it->first));
			return _serv->send_replies(_expeditor, " " + chan_it->first + " :End of who list", RPL_ENDOFWHO);
		}

		target->send_who_reply(_expeditor, NULL);
	}
	return _serv->send_replies(_expeditor, " * :End of who list", RPL_ENDOFWHO);
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