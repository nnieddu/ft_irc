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
	bool		addRule = false;
	int			num_o_b = 0;
	std::string	temp = arg;

	temp.clear();
	if (mod[0] == '+')
		addRule = true;
	mod.erase(0,1);
	if (_expeditor->isOperator(chan.getName()) && chan.getName()[0] != '+') {
		while (!mod.empty()) {
			if (mod[0] == 'l') {
				if (!addRule)
					chan.removeMod(l);
				else {
					chan.setMod(l);
					chan.setLim(atoi(temp.substr(0, temp.find_first_of(" \n\r", 0)).c_str()));
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
			}
			else if (mod[0] == 'o' && num_o_b < 3) {
				if (!addRule) {
					serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->demote(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
				else {
					serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->promote(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
				num_o_b++;
			}
			else if (mod[0] == 'b' && num_o_b < 3) {
				if (!addRule)
					chan.removeMod(b);
				else {
					chan.setMod(b);
					chan.setBanMask(temp.substr(0, temp.find_first_of(" \n\r", 0)));
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
				num_o_b++;
			}
			else if (mod[0] == 'k') {
				if (!addRule)
					chan.removeMod(k);
				else {
					chan.setMod(k);
					chan.setPass(temp.substr(0, temp.find_first_of(" \n\r", 0)));
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
			}
			else if (mod[0] == 'v') {
				//modification on user for this
			}
			else if (mod[0] == 'a') {
				if (chan.getName()[0] == '&' || (chan.getName()[0] == '!' && _expeditor == &chan.getChanCrea())) {
					if (!addRule)
						chan.removeMod(a);
					else
						chan.setMod(a);
				}
				else
					;//TODO NOT ENOUGH PERMISSION
			}
			else if (mod[0] == 'i') {
				if (!addRule)
					chan.removeMod(i);
				else
					chan.setMod(i);
			}
			else if (mod[0] == 'm') {
				if (!addRule)
					chan.removeMod(m);
				else
					chan.setMod(m);
			}
			else if (mod[0] == 'n') {
				if (!addRule)
					chan.removeMod(n);
				else
					chan.setMod(n);
			}
			else if (mod[0] == 'q') {
				if (!addRule)
					chan.removeMod(q);
				else
					chan.setMod(q);
			}
			else if (mod[0] == 'p') {
				if (!addRule)
					chan.removeMod(p);
				else
					chan.setMod(p);
			}
			else if (mod[0] == 's') {
				if (!addRule)
					chan.removeMod(s);
				else
					chan.setMod(s);
			}
			else if (mod[0] == 'r') {
				if (_expeditor == &chan.getChanCrea() && chan.)
				//TODO
				if (!addRule)
					chan.removeMod(r);
				else
					chan.setMod(r);
			}
			else if (mod[0] == 'e') {
				//TODO
				if (!addRule)
					chan.removeMod(e);
				else
					chan.setMod(e);
			}
			else if (mod[0] == 'I') {
				//TODO
				if (!addRule)
					chan.removeMod(I);
				else
					chan.setMod(I);
			}
			mod.erase(0, 1);
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
