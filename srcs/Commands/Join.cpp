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

	if (!channel)
	{
		_serv->send_replies(_expeditor, "JOIN :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}

	lst = _args[CHANNEL].parseList();

	while (!lst.empty())
	{
		std::string	name(nameCaseIns(lst.front()));
		if (name.find('#') != 0 && name.find('&') != 0 && name.find('+') != 0 && name.find('!') != 0)
		{
			_serv->send_replies(_expeditor, name + "::No such channel (need a chan mask)", ERR_BADCHANMASK);
			lst.erase(lst.begin());
			continue ;
		}
		if (name.size() == 1)
		{
			_serv->send_replies(_expeditor, name + "::Need a channel name (at least one character)", ERR_BADCHANMASK);
			lst.erase(lst.begin());
			continue ;
		}		
		if (_expeditor->getChannels().size() == 10)
		{
			_serv->send_replies(_expeditor, name + " :You have joined too many channels (10 max)", ERR_TOOMANYCHANNELS);
			return ;
		}
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
			if (_serv->getChannel(name)->getl() == true && _serv->getChannel(name)->getLim() >= _serv->getChannel(name)->getUsers().size())
			{	
				_serv->send_replies(_expeditor, name + " :Cannot join channel (+l)", ERR_CHANNELISFULL);
				lst.erase(lst.begin());
				continue ;
			}
			if (_serv->getChannel(name)->getk() == true)
			{	
				if (!keys)
				{
					_serv->send_replies(_expeditor, name + " :Cannot join channel (+k) need a key.", ERR_BADCHANNELKEY);
					lst.erase(lst.begin());
					continue ;
				}					
				else
				{
					std::deque<std::string>	lst_keys;
					lst_keys = _args[PASS].parseList();
					std::string	key(nameCaseIns(lst_keys.front()));
					if (_serv->getChannel(name)->getPass() != key)
					{
						_serv->send_replies(_expeditor, name + " :Cannot join channel (+k) wrong key.", ERR_BADCHANNELKEY);
						lst.erase(lst.begin());
						lst_keys.erase(lst.begin());
						continue ;
					}
					lst_keys.erase(lst.begin());
				}
			}
			if (_serv->getChannel(name)->geti() == true)
			{
				_serv->send_replies(_expeditor, name + " :Cannot join channel (+i)", ERR_INVITEONLYCHAN);
				lst.erase(lst.begin());
				continue ;
			}

			std::string msg;
			std::set<user *>::iterator it;
			for(it = _serv->channels[name]->getUsers().begin(); it != _serv->channels[name]->getUsers().end(); ++it)
			{
				msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
				send((*it)->getSock(), msg.c_str(), msg.length(), 0);
			}
			msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
			send(_expeditor->getSock(), msg.c_str(), msg.length(), 0);
			_serv->send_replies(_expeditor, + " " + name + " :" + _serv->channels[name]->getTopic(), RPL_TOPIC);
			_serv->getChannel(name)->send_names_replies(_expeditor);
			_serv->send_replies(_expeditor, name + " :End of names list", RPL_ENDOFNAMES);
			_expeditor->join_channel(name, false);
			_serv->channels[name]->addUser(*_expeditor);
		}

		lst.erase(lst.begin());
	}
}
// add operator replies when created/promote

// 474     ERR_BANNEDFROMCHAN
// "<channel> :Cannot join channel (+b)"