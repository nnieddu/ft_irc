#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Join::~Join(){}

Join & Join::operator=(const Join & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Join::Join(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
	_args[PASS].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void	Join::execute()
{
	std::string	*			channel = _args[CHANNEL].arg;
	std::string	*			keys = _args[PASS].arg;
	std::deque<std::string>	lst;
	std::deque<std::string>	lst_keys;

	if (!channel)
	{
		_serv->send_replies(_expeditor, "JOIN :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}

	lst = _args[CHANNEL].parseList();
	if (keys)
		lst_keys = _args[PASS].parseList();

	while (!lst.empty())
	{
		std::string	name(nameCaseIns(lst.front()));
		if (name.find('#') != 0 && name.find('&') != 0 && name.find('+') != 0 && name.find("!!") != 0)
		{
			if (_serv->channels.find(name) == _serv->channels.end())
			{
				_serv->send_replies(_expeditor, name + " :No such channel (need a chan mask)", ERR_BADCHANMASK);
				lst.erase(lst.begin());
				continue ;
			}
		}
		if (name.size() == 1 || (name.find("!!") == 0 && name.size() == 2))
		{
			_serv->send_replies(_expeditor, name + " :Need a channel name (at least one character)", ERR_BADCHANMASK);
			lst.erase(lst.begin());
			continue ;
		}		
		if (_expeditor->getChannels().size() == 10)
		{
			_serv->send_replies(_expeditor, name + " :You have joined too many channels (10 max)", ERR_TOOMANYCHANNELS);
			return ;
		}
		if (name.find("!!") == 0)
			name.erase(0,1);
		if (!_expeditor->isMember(name) && _serv->channels.find(name) == _serv->channels.end())
		{
			std::cout << "Channel : " << name << " created" << std::endl;
			_serv->channels[name] = new Channel(*_expeditor, name);
			_serv->channels[name]->addUser(*_expeditor);
			_expeditor->promote(name);
			_expeditor->join_channel(name, true);
			std::string replies = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
			send(_expeditor->getSock(), replies.c_str(), replies.length(), 0);
			_serv->getChannel(name)->send_names_replies(_expeditor);
			_serv->send_replies(_expeditor, name + " :End of names list", RPL_ENDOFNAMES);
		}
		else if (!_expeditor->isMember(name))
		{
			if (_serv->getChannel(name)->getl() && _serv->getChannel(name)->getLim() >= _serv->getChannel(name)->getUsers().size())
			{	
				_serv->send_replies(_expeditor, name + " :Cannot join channel (+l)", ERR_CHANNELISFULL);
				lst.erase(lst.begin());
				continue ;
			}
			if (_serv->getChannel(name)->getk())
			{	
				if (!keys)
				{
					_serv->send_replies(_expeditor, name + " :Cannot join channel (+k) need a key.", ERR_BADCHANNELKEY);
					lst.erase(lst.begin());
					continue ;
				}					
				else
				{
					std::string	key(nameCaseIns(lst_keys.front()));
					if (_serv->getChannel(name)->getPass() != key)
					{
						_serv->send_replies(_expeditor, name + " :Cannot join channel (+k) wrong key.", ERR_BADCHANNELKEY);
						lst.erase(lst.begin());
						lst_keys.erase(lst_keys.begin());
						continue ;
					}
					lst_keys.erase(lst_keys.begin());
				}
			}
			if (_serv->getChannel(name)->geti() && !_serv->getChannel(name)->isInvited(*_expeditor))
			{
				_serv->send_replies(_expeditor, name + " :Cannot join channel (+i)", ERR_INVITEONLYCHAN);
				lst.erase(lst.begin());
				continue ;
			}

			std::string msg;
			std::set<user *>::iterator it;
			for(it = _serv->channels[name]->getUsers().begin(); it != _serv->channels[name]->getUsers().end(); ++it)
			{
				if (!_serv->channels[name]->geta())
					msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
				else
					msg = ":" + _expeditor->getNickname(true) + " JOIN :" + name + "\r\n";
				send((*it)->getSock(), msg.c_str(), msg.length(), 0);
			}
			msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
			send(_expeditor->getSock(), msg.c_str(), msg.length(), 0);
			if (!_serv->channels[name]->getTopic().empty())
				_serv->send_replies(_expeditor, name + " :" + _serv->channels[name]->getTopic(), RPL_TOPIC);
			else
				_serv->send_replies(_expeditor, name + " :No topic is set", RPL_NOTOPIC);
			if (!_serv->channels[name]->geta())
			{
				_serv->getChannel(name)->send_names_replies(_expeditor);
				_serv->send_replies(_expeditor, name + " :End of names list", RPL_ENDOFNAMES);
			}
			_expeditor->join_channel(name, false);
			_serv->channels[name]->addUser(*_expeditor);
			if (_serv->getChannel(name)->isInvited(*_expeditor))
				_serv->getChannel(name)->remInvited(*_expeditor);
		}
		lst.erase(lst.begin());
	}
}
