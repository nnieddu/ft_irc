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
	if (!_serv->isUser(*usr))
		return _serv->send_replies(_expeditor, "INVITE :No such nick", ERR_NOSUCHNICK);
	//ERR_NOTONCHANNEL	?
	if (_serv->channels.find(*channel) != _serv->channels.end())
	{
		//if (_serv->getUser(*nick)->isMember(*channel))
		//	return _serv->send_replies(_expeditor, "INVITE :User already on channel", ERR_USERONCHANNEL);
		//if (/* channel has +i mode*/ && !(_serv->getUser(*nick)->isOperator(*channel)))
		//	return _serv->send_replies(_expeditor, "INVITE :Channel operator privilege is needed", ERR_CHANOPRIVSNEEDED);
	}
	if (_serv->getUser(*nick)->isAway())
		return _serv->send_replies(_expeditor, "INVITE :user is away", RPL_AWAY);
	//	return send RPL_INVITING apres tests pck je comprend pas
}
