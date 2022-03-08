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
	if (*arg == "anonymous" || arg->size() > 9) //// check char spe/grammar protocol rfc
	{
		_serv->send_replies(_expeditor, " :Erroneus nickname", ERR_ERRONEUSNICKNAME);
		return ;
	}
	if (_serv->isUser(*arg) == false)
	{
		if (_expeditor->getisLogged() == true)
		{
			std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *arg + "\r\n";
			send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
			_expeditor->setNickname(*arg);
		}
		else if (_expeditor->getisLogged() == false && _expeditor->getLogLvl() == 1)
		{
			_expeditor->setNickname(*arg);
			_expeditor->incLogLvl();
		}

		if (!_expeditor->getChannels().empty())
		{
			for (std::map<std::string, unsigned int>::iterator it = _expeditor->getChannels().begin() ; it != _expeditor->getChannels().end(); ++it)
			{
				if (!_serv->channels[it->first]->getUsers().empty())
				{	
					for (std::set<user*>::iterator it2 = _serv->channels[it->first]->getUsers().begin() ; it2 != _serv->channels[it->first]->getUsers().end(); ++it2)
					{
						std::cout << it->first << " TEST " << (*it2)->getNickname() << std::endl;
						std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *arg + "\r\n";
						send((*it2)->getSock(), str.c_str(), strlen(str.c_str()), 0);
					}
				}
			}
		}
	}
	else
		_serv->send_replies(_expeditor, *arg + " :Nickname is already in use", ERR_NICKNAMEINUSE);
}
// ERR_NICKCOLLISION osef ?
