#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Privmsg::~Privmsg() {}

Privmsg & Privmsg::operator=(const Privmsg & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Privmsg::Privmsg(server * serv):Command(serv)
{
	_args[RECEIVER].isNeeded	= true;
	_args[MESSAGE].isNeeded	= true;
}

/*----------------------------------------------------------------------------*/

void Privmsg::execute()
{
	std::string	*			receiver = _args[RECEIVER].arg;
	std::string	*			arg = _args[MESSAGE].arg;
	std::deque<std::string>	list;

	if (!receiver)
		return _serv->send_replies(_expeditor, "PRIVMSG :No receiver specified", ERR_NORECIPIENT);
	if (!arg)
		return _serv->send_replies(_expeditor, "PRIVMSG :No text to send", ERR_NOTEXTTOSEND);

	list = _args[RECEIVER].parseList();

	while (!list.empty())
	{
		if (HasChannelPrefix(list[0]))
		{
			if (_serv->channels.find(nameCaseIns(list[0])) != _serv->channels.end())
			{
				if (_serv->send_msg_to_channel(_expeditor, _serv->getChannel(nameCaseIns(list[0])), *arg) == -1)
					std::cerr << strerror(errno) << std::endl;
			}
			else if (_serv->send_msg_to_user(_expeditor, _serv->getUser(list[0]), *arg, "") == -1)
				std::cerr << strerror(errno) << std::endl;
			
		}
		else
		{
			user * usr = _serv->getUser(list[0]);

			if (_serv->send_msg_to_user(_expeditor, usr, *arg, "") == -1)
				std::cerr << strerror(errno) << std::endl;
		}
		list.pop_front();
	}
	return ;
}
