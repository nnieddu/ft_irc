//
// Created by Maximilien Rozniecki on 3/1/22.
//

#include "../incs/Topic.hpp"

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

int Topic::execute() {/*
	if (_arg) {
		if ()
	}
	else if (_channel) {

	}
	else {

	}*/
	return 0;
}
