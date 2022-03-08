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
}

/*----------------------------------------------------------------------------*/

void	Join::execute()
{
	std::string	*			channel = _args[CHANNEL].arg;
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
			_serv->send_replies(_expeditor, "No such channel (need a chan mask)", ERR_BADCHANMASK);
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
		else if (!_expeditor->isMember(name) && _serv->getChannel(name)->geti() == false)
		{
			// if (_serv->getChannel(name)->getk() == true && _serv->getChannel(name)->getPass() == arg)
			// {	
			// 	_serv->send_replies(_expeditor, name + " :Cannot join channel (+k)", ERR_INVITEONLYCHAN);
			// 	continue ;
			// }
			if (_serv->getChannel(name)->getl() == true && _serv->getChannel(name)->getLim() >= _serv->getChannel(name)->getUsers().size())
			{	
				_serv->send_replies(_expeditor, name + " :Cannot join channel (+l)", ERR_CHANNELISFULL);
				continue ;
			}
			_serv->send_replies(_expeditor, name + " :Cannot join channel (+k)", ERR_BADCHANNELKEY);
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
		if (_serv->getChannel(name)->geti() == true)
			_serv->send_replies(_expeditor, name + " :Cannot join channel (+i)", ERR_INVITEONLYCHAN);

		lst.erase(lst.begin());
	}
}
//ERR_BANNEDFROMCHAN
// add operator replies when created/promote