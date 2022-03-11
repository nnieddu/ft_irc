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
				(*it)->send_who_reply(_expeditor, NULL, _serv->getName());
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
			target->send_who_reply(_expeditor, _serv->getChannel(chan_it->first), _serv->getName());
			return _serv->send_replies(_expeditor, " " + chan_it->first + " :End of who list", RPL_ENDOFWHO);
		}

		target->send_who_reply(_expeditor, NULL, _serv->getName());
	}
	return _serv->send_replies(_expeditor, " * :End of who list", RPL_ENDOFWHO);
}
