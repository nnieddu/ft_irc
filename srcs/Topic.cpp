//
// Created by Maximilien Rozniecki on 3/1/22.
//

#include "../incs/Topic.hpp"
#include "../incs/Server.hpp"

//demander comment sont gerer les deux points
//exemple: TOPIC #test :

Topic::Topic() : Command() {
	_args[MESSAGE].isNeeded	= true;
	_args[CHANNEL].isNeeded		= true;
}

Topic::Topic(server *serv) : Command(serv) {
	_args[MESSAGE].isNeeded	= true;
	_args[CHANNEL].isNeeded		= true;
}

Topic::~Topic() {
}

Topic &Topic::operator=(const Topic &old) {
	if (this != &old)
		serv = old.serv;
	return *this;
}

int Topic::execute() {
	std::string	*	arg = _args[MESSAGE].arg;
	std::string	*	channel = _args[CHANNEL].arg;

	if (arg) {
		*channel = nameCaseIns(*channel);
		if (serv->channels.find(*channel) == serv->channels.end()) {
			send(_expeditor->getSock(), "Error: Channel doesn't exist\r\n", 30, 0);
			return -1;
		}
		if (!(serv->channels[*channel]->getMod() & t) || \
		_expeditor->getChannels()[*channel]) {
			if (*arg == ":")
				serv->channels[*channel]->setTopic(std::string("No topic set for channel ") + \
				(*channel)[0] + serv->channels[*channel]->getId() + channel->substr(1));
			else
				serv->channels[*channel]->setTopic(*arg);
		}
		else {
			send(_expeditor->getSock(), "Error: Need Op permission\r\n", 27, 0);
			return -1;
		}
	}
	else if (channel) {
		*channel = nameCaseIns(*channel);
		if (serv->channels.find(*channel) != serv->channels.end()) {
			send(_expeditor->getSock(), serv->channels[*channel]->getTopic().c_str(), \
			serv->channels[*channel]->getTopic().length(), 0);
		}
	}
	return 0;
}
