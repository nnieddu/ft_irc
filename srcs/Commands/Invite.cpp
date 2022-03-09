#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Invite::~Invite(){}

Invite & Invite::operator=(const Invite & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Invite::Invite(server * serv):Command(serv)
{
	_args[NICK].isNeeded = true;
	_args[CHANNEL].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Invite::execute()
{
	std::string *	nick = _args[NICK].arg;
	std::string *	channel = _args[CHANNEL].arg;

	if (!nick || !channel)
		return _serv->send_replies(_expeditor, "INVITE :Not enough parameters", ERR_NEEDMOREPARAMS);
	if (!_serv->isUser(*nick))
		return _serv->send_replies(_expeditor, "INVITE :No such nick or channel", ERR_NOSUCHNICK);
	if (!_expeditor->isMember(*channel))
		return _serv->send_replies(_expeditor, *channel + " :You're not on that channel", ERR_NOTONCHANNEL);
	if (_serv->getUser(*nick)->isAway())
		return _serv->send_replies(_expeditor, ":" + _serv->getUser(*nick)->getAfkString(), RPL_AWAY);
	if (_serv->channels.find(*channel) != _serv->channels.end())
	{
		if (_serv->getUser(*nick)->isMember(*channel))
			return _serv->send_replies(_expeditor, *nick + " " + *channel + " :is already on channel", ERR_USERONCHANNEL);
		if (_serv->getChannel(*channel)->geti() && !_expeditor->isOperator(*channel))
			return _serv->send_replies(_expeditor, *channel + " :You're not channel operator", ERR_CHANOPRIVSNEEDED);

		std::string replies = ":" + _expeditor->getNickname() + " INVITE " + *nick + " " + *channel + "\r\n";
		if (send(_serv->getUser(*nick)->getSock(), replies.c_str(), replies.length(), 0) == -1)
			std::cerr << strerror(errno) << std::endl;

		std::set<user*>	userlist(_serv->getChannel(*channel)->getUsers());
		for	(std::set<user*>::iterator it = userlist.begin(); it != userlist.end(); it++)
			_serv->send_replies((*it), *nick + " " + *channel, RPL_INVITING);

		_serv->getChannel(*channel)->addInvited(*_serv->getUser(*nick));
	}	
}
