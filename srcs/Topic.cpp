//
// Created by Maximilien Rozniecki on 3/1/22.
//

#include "../incs/Topic.hpp"
#include "../incs/Server.hpp"

Topic::Topic() : Command() {
	_argument	= true;
	_chan		= true;
}

Topic::Topic(server *serv) : Command(serv) {
	_argument	= true;
	_chan		= true;
}

Topic::~Topic() {
}

Topic &Topic::operator=(const Topic &old) {
	if (this != &old)
		serv = old.serv;
	return *this;
}

int Topic::execute() {
	if (_arg) {
		if (serv->channels.find(*_channel) == serv->channels.end()) {
			send(_expeditor->getSock(), "Error: Channel doesn't exist\r\n", 30, 0);
			return -1;
		}
		if (!(serv->channels[*_channel]->getMod() & t) || \
		_expeditor->getChannels()[*_channel])
			serv->channels[*_channel]->setTopic(*_arg);
		else {
			send(_expeditor->getSock(), "Error: Need Op permission\r\n", 27, 0);
			return -1;
		}
	}
	else if (_channel) {
		if (serv->channels.find(*_channel) == serv->channels.end()) {
			if (!(serv->channels[_expeditor->getLocation()]->getMod() & t) || \
			_expeditor->getChannels()[*_channel])
				serv->channels[_expeditor->getLocation()]->setTopic(*_channel);
		}
		else {
			send(_expeditor->getSock(), serv->channels[*_channel]->getTopic().c_str(), \
			serv->channels[*_channel]->getTopic().length(), 0);
		}
	}
	else {
		send(_expeditor->getSock(), serv->channels[_expeditor->getLocation()]->getTopic().c_str(), \
			serv->channels[_expeditor->getLocation()]->getTopic().length(), 0);
	}
	return 0;
}
