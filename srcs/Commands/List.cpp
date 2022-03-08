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
	std::map<std::string, Channel*>::iterator it;
	_serv->send_replies(_expeditor, "", RPL_LISTSTART);
	if (!arg)
	{
		for (it = _serv->channels.begin(); it != _serv->channels.end(); ++it)
		{
			if (it->second->getp() == false)
				_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->first.size()) + " " + it->second->getTopic() , RPL_LIST);
			if (it->second->getp() == true && _expeditor->isMember(it->first) == false)
				_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->first.size()) + " Prv" , RPL_LIST);
			else if (_expeditor->isMember(it->first) == true)
				_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->first.size()) + " " + it->second->getTopic() , RPL_LIST);
		}
	}
	else if (arg && (_serv->channels.find(*arg) != _serv->channels.end()))
	{
		it = _serv->channels.find(*arg);
		if (it->second->getp() == false)
			_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->first.size()) + " " + it->second->getTopic() , RPL_LIST);
		if (it->second->getp() == true && _expeditor->isMember(it->first) == false)
			_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->first.size()) + " Prv" , RPL_LIST);
		else if (_expeditor->isMember(it->first) == true)
			_serv->send_replies(_expeditor, it->first + " " + ft_itoa(it->first.size()) + " " + it->second->getTopic() , RPL_LIST);
	}
	_serv->send_replies(_expeditor, ":End of /LIST", RPL_LISTEND);
}
