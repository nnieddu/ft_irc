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

void Mode::modeChan(Channel& chan, std::string &mod, std::string &arg) {
	bool		addRule = false;
	int			num_o_b = 0;
	std::string	temp = arg;

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
			}/*
			else if (mod[0] == 'b' && num_o_b < 3) {
				if (!addRule)
					chan.removeMod(b);
				else {
					chan.setMod(b);
					chan.setBanMask(temp.substr(0, temp.find_first_of(" \n\r", 0)));
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
				num_o_b++;
			}*/
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
				if (!addRule) {
					serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->delVoice(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
				else {
					serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->setVoice(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
				}
			}
			else if (mod[0] == 'a') {
				if (chan.getName()[0] == '&' || (chan.getName()[0] == '!' && _expeditor == &chan.getChanCrea())) {
					if (!addRule)
						chan.removeMod(a);
					else
						chan.setMod(a);
				}
				else
					std::cerr << "TODO this case Mode.cpp";//TODO NOT ENOUGH PERMISSION OR NOT AVAILABLE FOR THIS CHANNEL
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
				if (_expeditor == &chan.getChanCrea() && chan.getName()[0] == '!') {
					if (!addRule)
						chan.removeMod(r);
					else
						chan.setMod(r);
				}
				else
					std::cerr << "TODO message d' error pour Mode r" << std::endl;
			}/*
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
			}*/
			mod.erase(0, 1);
		}

	}
	return ;
}

void Mode::modeUser(user& usr, std::string &mod, std::string &arg) {
	bool		addRule = false;
	std::string	temp = arg;
	user*		target = serv->getUser(temp.substr(0, temp.find_first_of(" \n\r", 0)));

	temp.erase(0, temp.find_first_of(" \n\r", 0));
	if (mod[0] == '+')
		addRule = true;
	mod.erase(0,1);

	if (_expeditor->isServOp() || _expeditor == target) {
		if ((mod[0] == 'o' || mod[0] == 'O') && _expeditor->isServOp()) {
			if (!addRule)
				target->demoteServ();
			else
				target->promoteServ();
		}
		else if (mod[0] == 'i') {
			if (!addRule)
				target->delInvisible();
			else
				target->setInvisible();
		}
		else if (mod[0] == 'w') {
			if (!addRule)
				target->delWallOp();
			else
				target->setWallOp();
		}
		else if (mod[0] == 'r') {
			if (!addRule && target != _expeditor)
				target->delRestrict();
			else
				target->setRestrict();
		}
		mod.erase(0, 1);
	}
	return ;
}

void Mode::execute() {
	std::string	*arg = _args[MESSAGE].arg;
	std::string	*mod = _args[USER].arg;
	std::string	*channel = _args[CHANNEL].arg;
	std::string *users = _args[CHANNEL].arg;

	if (channel) {
		if (serv->channels.find(nameCaseIns(*channel)) != serv->channels.end())
			return modeChan(*serv->channels[nameCaseIns(*channel)], *mod, *arg);
		else if (serv->getUser(*users) != NULL)
			return modeUser(*serv->getUser(*users), *mod, *arg);
	}
	return ;
}
