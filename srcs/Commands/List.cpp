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
	_serv->send_replies(_expeditor, "Channel :Users  Name", RPL_LISTSTART);
	if (!arg)
	{
		for (it = _serv->channels.begin(); it != _serv->channels.end(); ++it)
		{
			// _serv->send_replies(_expeditor, it->first + " <# visible> :" + it->second->getTopic() , RPL_LIST);
			_serv->send_replies(_expeditor, it->first + ":" + it->second->getTopic() , RPL_LIST);
		}
	}
	if (arg && (_serv->channels.find(*arg) != _serv->channels.end()))
	{
		it = _serv->channels.find(*arg);
		_serv->send_replies(_expeditor, it->first + " <# visible> :" + it->second->getTopic() , RPL_LIST);
	}
	_serv->send_replies(_expeditor, ":End of /LIST", RPL_LISTEND);

	return ;
}
