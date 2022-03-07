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
	_args[LIST].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void	Join::execute()
{
	std::string	*			channel = _args[CHANNEL].arg;
	std::string	*			list = _args[LIST].arg;
	std::deque<std::string>	lst;

	if (!channel)
	{
		_serv->send_replies(_expeditor, "JOIN :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}
	if (list)
	{
		lst = _args[LIST].parseList();
		lst.push_front(*channel);
	}
	else
		lst = _args[CHANNEL].parseList();

	while (!lst.empty())
	{
		std::string	name(nameCaseIns(lst.front()));

		if (name.find('#') && name.find('&') == std::string::npos && name.find('+') == std::string::npos
		&& name.find('!') == std::string::npos)
		{
			_serv->send_replies(_expeditor, "No such channel (need a chan mask)", ERR_BADCHANMASK);
			return ;
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
			_expeditor->join_channel(name, true);
			_serv->channels[name]->addUser(*_expeditor);
		}
		lst.erase(lst.begin());
	}
	return ;
}
//ERR_BANNEDFROMCHAN
//ERR_INVITEONLYCHAN
//ERR_CHANNELISFULL
//ERR_NOSUCHCHANNEL
//ERR_BADCHANNELKEY
