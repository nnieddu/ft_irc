//
// Created by Maximilien Rozniecki on 3/1/22.
//

#include "../incs/Topic.hpp"
#include "../incs/Server.hpp"

//demander comment sont gerer les deux points
//exemple: TOPIC #test :

Topic::Topic() : Command() {
	_args[ARGUMENT].isNeeded	= true;
	_args[CHANNEL].isNeeded		= true;
}

Topic::Topic(server *serv) : Command(serv) {
	_args[ARGUMENT].isNeeded	= true;
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
	std::string	*	arg = _args[ARGUMENT].arg;
	std::string	*	channel = _args[CHANNEL].arg;

	if (arg) {
		if (serv->channels.find(*channel) == serv->channels.end()) {
			send(_expeditor->getSock(), "Error: Channel doesn't exist\r\n", 30, 0);
			return -1;
		}
		if (!(serv->channels[*channel]->getMod() & t) || \
		_expeditor->getChannels()[*channel]) {
			if (*arg == ":")
				serv->channels[*channel]->setTopic(std::string("No topic set for channel ") + \
				(*channel)[0] + serv->channels[*channel]->getId() + channel->substr(1));
			else {
				if ((*arg)[0] == ':')
					(*arg) = arg->substr(1);
				serv->channels[*channel]->setTopic(*arg);
			}
		}
		else {
			send(_expeditor->getSock(), "Error: Need Op permission\r\n", 27, 0);
			return -1;
		}
	}
	else if (channel) {
		if (serv->channels.find(*channel) != serv->channels.end()) {
			send(_expeditor->getSock(), serv->channels[*channel]->getTopic().c_str(), \
			serv->channels[*channel]->getTopic().length(), 0);
		}
	}
	return 0;
}
