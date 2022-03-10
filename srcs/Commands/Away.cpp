//
// Created by Maximilien Rozniecki on 3/10/22.
//

#include "../../incs/Away.hpp"

Away::Away(server *serv) : Command(serv){
	_args[MESSAGE].isNeeded	= true;
}

Away::~Away() {

}

Away &Away::operator=(const Away &old) {
	if (this != &old)
		_serv = old.getServ();
	return *this;
}

void Away::execute() {
	std::string	*	awayMsg = _args[MESSAGE].arg;

	if (!awayMsg) {
		_expeditor->delAfk();
		_serv->send_replies(_expeditor, ":You are no longer marked as being away", RPL_UNAWAY);
	}
	else {
		_expeditor->setAfk(*awayMsg);
		_serv->send_replies(_expeditor, ":You have been marked as being away", RPL_NOWAWAY);
	}
}
