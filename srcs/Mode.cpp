//
// Created by fenrir on 02/03/2022.
//

#include "../incs/Mode.hpp"

Mode::Mode() : Command() {
	_args[MESSAGE].isNeeded	= true;
	_args[USER].isNeeded		= true;
	_args[CHANNEL].isNeeded		= true;
}

Mode::Mode(server *serv) : Command(serv){
	_args[MESSAGE].isNeeded	= true;
	_args[USER].isNeeded		= true;
	_args[CHANNEL].isNeeded		= true;
}

Mode::~Mode() {

}

Mode &Mode::operator=(const Mode &old) {
	if (this != &old)
		serv = old.serv;
	return *this;
}

int Mode::modeChan(Channel& chan, std::string &mod, std::string &arg) {
	bool	addRule = false;

	if (mod[0] == '+')
		addRule = true;
	mod.erase(0,1);
	if (_expeditor->getChannels()[chan.getName()]) {
		for (int index = 0; mod[index] ; ++index) {

		}
	}
	return 0;
}

int Mode::modeUser(user& usr, std::string &mod, std::string &arg) {
	return 0;
}

int Mode::execute() {
	std::string	*arg = _args[MESSAGE].arg;
	std::string	*mod = _args[USER].arg;
	std::string	*channel = _args[CHANNEL].arg;

	if (channel) {
		if (serv->channels.find(*channel) != serv->channels.end())
			return modeChan(*serv->channels[*channel], *mod, *arg);
		else if (serv->getUser(*channel) != NULL)
			return modeUser(*serv->getUser(*channel), *mod, *arg);
	}
	return 0;
}
