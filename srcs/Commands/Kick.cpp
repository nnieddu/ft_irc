#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Kick::~Kick(){}

Kick & Kick::operator=(const Kick & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Kick::Kick(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
	_args[USER].isNeeded = true;
	_args[MESSAGE].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void	Kick::execute()
{
	std::string	*	msg = _args[MESSAGE].arg;

	if (!_args[CHANNEL].arg || !_args[USER].arg)
		return _serv->send_replies(_expeditor, "KICK :Not enough parameters", ERR_NEEDMOREPARAMS);
	
	std::deque<std::string>	chan_list = _args[CHANNEL].parseList();
	std::deque<std::string>	user_list = _args[USER].parseList();

	while (!chan_list.empty())
	{
		bool	out = false;

		for (std::deque<std::string>::iterator	it(user_list.begin()); out == false && it != user_list.end(); it++)
		{
			if (_serv->channels.find(nameCaseIns(chan_list.front())) == _serv->channels.end())
				return _serv->send_replies(_expeditor, "KICK :No such channel", ERR_NOSUCHCHANNEL);
			if (!(_expeditor->isOperator(chan_list.front())))
			{
				_serv->send_replies(_expeditor, "KICK :Channel operator privilege is needed", ERR_CHANOPRIVSNEEDED);
				out = true;
			}
			if (!out && !(_expeditor->isMember(chan_list.front())))
			{
				_serv->send_replies(_expeditor, "KICK :You are not on channel", ERR_NOTONCHANNEL);
				out = true;
			}
			if (!out)
			{
				std::set<user *>	chan_userlist = _serv->channels.find(nameCaseIns(chan_list.front()))->second->getUsers();
				user *				target = _serv->getUser(*it);

				_serv->remove_user_from(target, chan_list.front(), "KICK");

				std::string kick = ":" + _expeditor->getNickname() + " KICK " + chan_list.front() + " " + target->getNickname() + ":" + (msg != NULL ? *msg : "") + "\r\n";
				for (std::set<user *>::iterator it(chan_userlist.begin()); it != chan_userlist.end(); it++)
				{
					send((*it)->getSock(), kick.c_str(), kick.length(), 0);
				}
			}
		}
		chan_list.pop_front();
	}
}
