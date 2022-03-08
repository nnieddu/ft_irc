//
// Created by Maximilien Rozniecki on 3/1/22.
//

#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Topic::Topic(server *serv) : Command(serv) {
	_args[MESSAGE].isNeeded	= true;
	_args[CHANNEL].isNeeded	= true;
}

Topic::~Topic() {
}

Topic &Topic::operator=(const Topic &old) {
	if (this != &old)
		_serv = old.getServ();
	return *this;
}

/*----------------------------------------------------------------------------*/

void Topic::execute() {
	std::string	*	arg = _args[MESSAGE].arg;
	std::string	*	channel = _args[CHANNEL].arg;

	if (arg) {
		*channel = nameCaseIns(*channel);
		if (_serv->channels.find(*channel) == _serv->channels.end())
			_serv->send_replies(_expeditor, (*channel) + " :No such channel", ERR_NOSUCHCHANNEL);
		else if ((*channel)[0] == '+')
			_serv->send_replies(_expeditor, (*channel) + " :Channel doesn't support modes", ERR_NOCHANMODES);
		else if (!_expeditor->isMember(*channel))
			_serv->send_replies(_expeditor, (*channel) + " :You're not on that channel", ERR_NOTONCHANNEL);
		else if (!_expeditor->isOperator(*channel) && (_serv->channels[*channel]->getMod() & t))
			_serv->send_replies(_expeditor, (*channel) + " :You do not have acces to change the topic on this channel", ERR_CHANOPRIVSNEEDED);
		else
			_serv->channels[*channel]->setTopic(*arg);
	}
	else if (channel) {
		*channel = nameCaseIns(*channel);
		if (_serv->channels.find(*channel) != _serv->channels.end()) {
			if (!_serv->channels[*channel]->getTopic().empty())
				_serv->send_replies(_expeditor, (*channel) + " :" + _serv->channels[*channel]->getTopic(), RPL_TOPIC);
			else
				_serv->send_replies(_expeditor, (*channel) + " :No topic is set", RPL_NOTOPIC);
		}
		else
			_serv->send_replies(_expeditor, (*channel) + " :No such channel", ERR_NOSUCHCHANNEL);
	}
	else
		_serv->send_replies(_expeditor, "TOPIC :Not enough parameters", ERR_NEEDMOREPARAMS);
}
