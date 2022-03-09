#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

List::~List(){}

List & List::operator=(const List & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

List::List(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void List::execute()
{
	std::string	*	arg = _args[CHANNEL].arg;

	std::string list_msg;
	std::string topic;
	std::map<std::string, Channel*>::iterator it;

	_serv->send_replies(_expeditor, "Channel :Users  Name", RPL_LISTSTART);
	if (!arg)
	{
		for (it = _serv->channels.begin(); it != _serv->channels.end(); ++it)
		{
			
			if (!it->second->getTopic().empty())
				topic = it->second->getTopic();
			else
				topic = "No topic is set";

			if (it->second->getp() == false)
				_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->second->getUsers().size()) + " " + topic, RPL_LIST);
			if (it->second->getp() == true && _expeditor->isMember(it->first) == false)
				_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->second->getUsers().size()) + " Prv" , RPL_LIST);
			else if (it->second->getp() == true && _expeditor->isMember(it->first) == false)
				_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->second->getUsers().size()) + " " + topic, RPL_LIST);
		}
	}
	else if (arg && (_serv->channels.find(*arg) != _serv->channels.end()))
	{
		it = _serv->channels.find(*arg);

		if (!it->second->getTopic().empty())
			topic = it->second->getTopic();
		else
			topic = "No topic is set";

		if (it->second->getp() == false)
			_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->second->getUsers().size()) + " " + topic, RPL_LIST);
		if (it->second->getp() == true && _expeditor->isMember(it->first) == false)
			_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->second->getUsers().size()) + " Prv" , RPL_LIST);
		else if (it->second->getp() == true && _expeditor->isMember(it->first) == false)
			_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->second->getUsers().size()) + " " + topic, RPL_LIST);
	}
	_serv->send_replies(_expeditor, ":End of /LIST", RPL_LISTEND);
}
