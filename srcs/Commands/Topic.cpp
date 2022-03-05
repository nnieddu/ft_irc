//
// Created by Maximilien Rozniecki on 3/1/22.
//

#include "../../incs/Commands.hpp"

//demander comment sont gerer les deux points
//exemple: TOPIC #test :

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
		if (_serv->channels.find(*channel) == _serv->channels.end()) {
			send(_expeditor->getSock(), "Error: Channel doesn't exist\r\n", 30, 0);
			return ;
		}
		if (!(_serv->channels[*channel]->getMod() & t) || \
		_expeditor->getChannels()[*channel]) {
			if (*arg == ":")
				_serv->channels[*channel]->setTopic(std::string("No topic set for channel ") + \
				(*channel)[0] + _serv->channels[*channel]->getId() + channel->substr(1));
			else
				_serv->channels[*channel]->setTopic(*arg);
		}
		else {
			send(_expeditor->getSock(), "Error: Need Op permission\r\n", 27, 0);
			return ;
		}
	}
	else if (channel) {
		*channel = nameCaseIns(*channel);
		if (_serv->channels.find(*channel) != _serv->channels.end()) {
			send(_expeditor->getSock(), _serv->channels[*channel]->getTopic().c_str(), \
			_serv->channels[*channel]->getTopic().length(), 0);
		}
	}
	return ;
}
