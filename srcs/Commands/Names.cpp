#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Names::~Names() {}

Names & Names::operator=(const Names & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Names::Names(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Names::execute()
{
	std::string	*		channel = _args[CHANNEL].arg;
	std::stringstream	output;

	if (channel)
	{
		std::deque<std::string> list = _args[CHANNEL].parseList();

		while (!list.empty())
		{
			std::map<std::string, Channel*>::iterator	it(_serv->channels.find(list.front()));

			if (it != _serv->channels.end())
				it->second->send_names_replies(_expeditor);
			list.pop_front();
		}
	}
	else
	{
		std::string	names;

		for (std::map<std::string, Channel* >::iterator it(_serv->channels.begin()); it != _serv->channels.end(); it++)
			it->second->send_names_replies(_expeditor);
		for (std::vector<user*>::iterator it(_serv->getUsers().begin()); it != _serv->getUsers().end(); it++)
		{
			if ((*it)->getChannels().empty())
				names += (*it)->getNickname() + " ";
		}
		if (!(names.empty()))
		{
			std::string replies = ":" + _expeditor->getUsername() +  " " + RPL_NAMREPLY + " " + _expeditor->getNickname() + " " +  "= * :" + names + "\r\n";
			send(_expeditor->getSock(), replies.c_str(), replies.length(), 0);
		}
	}
	_serv->send_replies(_expeditor, "* :End of names list", RPL_ENDOFNAMES);
	return ;
}
