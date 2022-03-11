#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Nick::~Nick() {}

Nick & Nick::operator=(const Nick & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Nick::Nick(server * serv):Command(serv)
{
	_args[NICK].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Nick::execute()
{
	std::string	*	arg = _args[NICK].arg;

	if (!arg)
	{
		_serv->send_replies(_expeditor, " :No nickname given", ERR_NONICKNAMEGIVEN);
		return ;
	}
	if (*arg == "anonymous" || arg->size() > 9 || !arg->find('?') || !arg->find('!') || !arg->find('(') 
		|| !arg->find(')') || !arg->find('#') || !arg->find('~') || !arg->find('\'') || !arg->find(',') 
			|| !arg->find('.') || !arg->find('$') || !arg->find('%') || !arg->find('*') || !arg->find('+')
				|| !arg->find('=') || !arg->find(':') || !arg->find(';') || !arg->find('\"') || !arg->find('<') 
					|| !arg->find('>') || !arg->find('/') || !arg->find('&') )
	{
		_serv->send_replies(_expeditor, " :Erroneus nickname", ERR_ERRONEUSNICKNAME);
		return ;
	}
	std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *arg + "\r\n";
	if (_serv->isUser(*arg) == false)
	{
		if (_expeditor->getisLogged() == true)
		{
			send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
			_expeditor->setNickname(*arg);
		}
		else if (_expeditor->getisLogged() == false && _expeditor->getLogLvl() == 1)
		{
			_expeditor->setNickname(*arg);
			_expeditor->setLogLvl(2);
		}

		if (!_expeditor->getChannels().empty())
		{
			std::map<std::string, unsigned int> channels = _expeditor->getChannels();

			for (std::map<std::string, unsigned int>::iterator it = channels.begin() ; it != channels.end(); it++)
			{
				std::set<user*>	users = _serv->channels[it->first]->getUsers();
	
				for (std::set<user*>::iterator it2 = users.begin() ; it2 != users.end(); it2++)
				{
					if (_expeditor != *it2)
						send((*it2)->getSock(), str.c_str(), strlen(str.c_str()), 0);
				}
			}
		}
	}
	else
		_serv->send_replies(_expeditor, *arg + " :Nickname is already in use", ERR_NICKNAMEINUSE);
}