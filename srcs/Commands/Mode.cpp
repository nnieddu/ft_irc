//
// Created by fenrir on 02/03/2022.
//

#include "../../incs/Mode.hpp"

/*----------------------------------------------------------------------------*/

Mode::Mode(server *serv) : Command(serv){
	_args[REST].isNeeded	= true;
	_args[USER].isNeeded		= true;
	_args[RECEIVER].isNeeded		= true;
}

Mode::~Mode() {}

Mode &Mode::operator=(const Mode &old) {
	if (this != &old)
		_serv = old._serv;
	return *this;
}

/*----------------------------------------------------------------------------*/

void Mode::modeChan(Channel& chan, std::string &mod, std::string &arg, std::string &defRep) {
	bool		addRule = false;
	int			num_o_b = 0;
	std::string	temp = arg;

	if (mod[0] == 'O') {
		if (chan.getName()[0] == '!') {
			_serv->send_replies(_expeditor, chan.getName() + " " + \
			chan.getChanCrea().getNickname(), RPL_UNIQOPIS);
		}
		else {
			_serv->send_replies(_expeditor, std::string() + mod[0] + \
			" :is unknown mode char to me for ", ERR_UNKNOWNMODE);
			return;
		}
		mod.erase(0, 1);
	}
	if (_expeditor->isOperator(chan.getName())) {
		while (!mod.empty()) {
			if (mod[0] == '-')
				addRule = false;
			else if (mod[0] == '+')
				addRule = true;
			else if (mod[0] == 'l') {
				if (!addRule)
					chan.removeMod(l);
				else {
					if (temp.empty()) {
						_serv->send_replies(_expeditor, chan.getName() + \
						" +l :Not enough parameters", ERR_NEEDMOREPARAMS);
						return;
					}
					chan.setMod(l);
					chan.setLim(atoi(temp.substr(0, temp.find_first_of(" \n\r", 0)).c_str()));
					temp.erase(0, temp.find_first_of(" \n\r", 0));
					if (temp[0] == ' ')
						temp.erase(0, 1);
				}
			}
			else if (mod[0] == 'o') {
				if (num_o_b > 3) {
					_serv->send_replies(_expeditor, chan.getName() + \
						" +o :Called more than 3 time in one MODE command", ERR_NEEDMOREPARAMS);
					return;
				}
				if (temp.empty()) {
					_serv->send_replies(_expeditor, chan.getName() + \
						" +o :Not enough parameters", ERR_NEEDMOREPARAMS);
					return;
				}
				if (_serv->getUser(std::string(temp.substr(0, temp.find_first_of(" \n\r", 0)))) == NULL) {
					_serv->send_replies(_expeditor, (std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0)))) + " :No such nick", ERR_NOSUCHNICK);
					return;
				}
				if (!addRule) {
					_serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->demote(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
					if (temp[0] == ' ')
						temp.erase(0, 1);
				}
				else {
					_serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->promote(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
					if (temp[0] == ' ')
						temp.erase(0, 1);
				}
				num_o_b++;
			}
			else if (mod[0] == 'k') {
				if (!addRule)
					chan.removeMod(k);
				else {
					if (chan.getk()) {
						_serv->send_replies(_expeditor, chan.getName() + \
						" :Channel key already set", ERR_KEYSET);
						return;
					}
					if (temp.empty()) {
						_serv->send_replies(_expeditor, chan.getName() + \
						" +k :Not enough parameters", ERR_NEEDMOREPARAMS);
						return;
					}
					chan.setMod(k);
					chan.setPass(temp.substr(0, temp.find_first_of(" \n\r", 0)));
					temp.erase(0, temp.find_first_of(" \n\r", 0));
					if (temp[0] == ' ')
						temp.erase(0, 1);
				}
			}
			else if (mod[0] == 'v') {
				if (temp.empty()) {
					_serv->send_replies(_expeditor, chan.getName() + \
						" v :Not enough parameters", ERR_NEEDMOREPARAMS);
					return;
				}
				if (_serv->getUser(std::string(temp.substr(0, temp.find_first_of(" \n\r", 0)))) == NULL) {
					_serv->send_replies(_expeditor, (std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0)))) + " :No such nick", ERR_NOSUCHNICK);
					return;
				}
				if (!addRule) {
					_serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->delVoice(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
					if (temp[0] == ' ')
						temp.erase(0, 1);
				}
				else {
					_serv->getUser(std::string(temp.substr(0, \
					temp.find_first_of(" \n\r", 0))))->setVoice(chan.getName());
					temp.erase(0, temp.find_first_of(" \n\r", 0));
					if (temp[0] == ' ')
						temp.erase(0, 1);
				}
			}
			else if (mod[0] == 'a') {
				if (chan.getName()[0] == '&' || (chan.getName()[0] == '!' && _expeditor == &chan.getChanCrea())) {
					if (!addRule)
					{
						chan.removeMod(a);
						anonMode(false, chan);
					}
					else
					{
						chan.setMod(a);
						anonMode(true, chan);
					}
				}
				else if (chan.getName()[0] == '!' && _expeditor != &chan.getChanCrea()) {
					_serv->send_replies(_expeditor, chan.getName() + " :You're not channel" + \
					" creator", ERR_CHANOPRIVSNEEDED);
					return;
				}
				else{
					_serv->send_replies(_expeditor, std::string() + mod[0] + \
					" :is unknown mode char to me for " + chan.getName(), ERR_UNKNOWNMODE);
					return;
				}
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
			else if (mod[0] == 'r') {
				if (_expeditor == &chan.getChanCrea() && chan.getName()[0] == '!') {
					if (!addRule)
						chan.removeMod(r);
					else
						chan.setMod(r);
				}
				else if (chan.getName()[0] == '!' && _expeditor != &chan.getChanCrea()) {
					_serv->send_replies(_expeditor, chan.getName() + " :You're not channel" + \
					" creator", ERR_CHANOPRIVSNEEDED);
					return;
				}
				else{
					_serv->send_replies(_expeditor, std::string() + mod[0] + \
					" :is unknown mode char to me for " + chan.getName(), ERR_UNKNOWNMODE);
					return;
				}
			}
			else {
				_serv->send_replies(_expeditor, std::string() + mod[0] + \
				" :is unknown mode char to me for " + chan.getName(), ERR_UNKNOWNMODE);
				return;
			}
			mod.erase(0, 1);
		}
		for (std::set<user*>::iterator it = chan.getUsers().begin(); \
		it != chan.getUsers().end() ; it++) {
			send((*it)->getSock(), defRep.c_str(), defRep.length(), 0);
		}
	}
	else {
		_serv->send_replies(_expeditor, chan.getName() + " :You're not channel" + \
		" operator", ERR_CHANOPRIVSNEEDED);
	}
}

/*----------------------------------------------------------------------------*/

void Mode::modeUser(user& usr, std::string &mod, std::string &arg, std::string &defRep) {
	int		addRule = true;

	if (_expeditor->isServOp() || _expeditor == &usr) {
		while (!mod.empty()) {
			if (mod[0] == '+')
				addRule = true;
			else if (mod[0] == '-')
				addRule = false;
			else if ((mod[0] == 'o' || mod[0] == 'O') && _expeditor->isServOp()) {
				if (!addRule)
					usr.demoteServ();
				else
					usr.promoteServ();
			}
			else if (mod[0] == 'i') {
				if (!addRule)
					usr.delInvisible();
				else
					usr.setInvisible();
			}
			else if (mod[0] == 'w') {
				if (!addRule)
					usr.delWallOp();
				else
					usr.setWallOp();
			}
			else if (mod[0] == 'r') {
				if (!addRule && &usr != _expeditor && _expeditor->isServOp())
					usr.delRestrict();
				else
					usr.setRestrict();
			}
			else {
				_serv->send_replies(_expeditor, std::string() + mod[0] + \
				" :is unknown mode char to me for ", ERR_UNKNOWNMODE);
				return;
			}
			mod.erase(0, 1);
		}
		send(_expeditor->getSock(), defRep.c_str(), defRep.length(), 0);
	}
	else
		_serv->send_replies(_expeditor, " :Cannot change mode for other users", ERR_USERSDONTMATCH);
}

/*----------------------------------------------------------------------------*/

void Mode::execute() {
	std::string	*arg = _args[REST].arg;
	std::string	*mod = _args[USER].arg;
	std::string	*receiver = _args[RECEIVER].arg;
	std::string emptyArg;
	std::string defReply;
	if (receiver) {
		defReply = std::string(":") + _expeditor->getNickname() + " MODE " + *receiver;
		emptyArg.clear();
		if (_serv->getUser(*receiver) != NULL) {
			if (mod == NULL) {
				_serv->send_replies(_expeditor, \
				receivModeIs(*_serv->getUser(*receiver)), RPL_UMODEIS);
			}
			else if (mod->find_first_not_of("+-") == std::string::npos)
				_serv->send_replies(_expeditor, "MODE :Not enough parameters", ERR_NEEDMOREPARAMS);
			else if (arg == NULL) {
				defReply += " :" + *mod + "\r\n";
				modeUser(*_serv->getUser(*receiver), *mod, emptyArg, defReply);
			}
			else {
				defReply += " " + *mod + " :" + *arg + "\r\n";
				modeUser(*_serv->getUser(*receiver), *mod, *arg, defReply);
			}
		}
		else if (_serv->channels.find(nameCaseIns(*receiver)) != _serv->channels.end()) {
			if ((*receiver)[0] == '+') {
				_serv->send_replies(_expeditor, (*receiver) + \
				" :Channel doesn't support modes", ERR_NOCHANMODES);
			}
			else if (!_expeditor->isMember(*receiver)) {
				_serv->send_replies(_expeditor, (*receiver) + \
				" :You're not on that channel", ERR_NOTONCHANNEL);
			}
			else if (_expeditor->isRestrict()) {
				_serv->send_replies(_expeditor, " :You are a restricted user", ERR_RESTRICTED);
			}
			else if (mod == NULL) {
				_serv->send_replies(_expeditor, \
				receivModeIs(*_serv->channels[nameCaseIns(*receiver)]), RPL_CHANNELMODEIS);
			}
			else if (mod->find_first_not_of("+-") == std::string::npos)
				_serv->send_replies(_expeditor, "MODE :Not enough parameters", ERR_NEEDMOREPARAMS);
			else if (arg == NULL) {
				defReply += " :" + *mod + "\r\n";
				modeChan(*_serv->channels[nameCaseIns(*receiver)], *mod, emptyArg, defReply);
			}
			else {
				defReply += " " + *mod + " :" + *arg + "\r\n";
				modeChan(*_serv->channels[nameCaseIns(*receiver)], *mod, *arg, defReply);
			}
		}
		else {
			_serv->send_replies(_expeditor, (*receiver) + \
			" :No such nick/channel", ERR_NOSUCHNICK);
		}
	}
	else
		_serv->send_replies(_expeditor, "MODE :Not enough parameters", ERR_NEEDMOREPARAMS);
}

std::string Mode::receivModeIs(Channel& chan) {
	std::string ret = chan.getName() + " +";
	std::string arg;
	arg.clear();
	if (chan.getn())
		ret += 'n';
	if (chan.geta())
		ret += 'a';
	if (chan.geti())
		ret += 'i';
	if (chan.getk()) {
		ret += 'k';
		arg += chan.getPass();
	}
	if (chan.getl()) {
		ret += 'l';
		arg += std::to_string(chan.getLim());
	}
	if (chan.getm())
		ret += 'm';
	if (chan.getp())
		ret += 'p';
	if (chan.getq())
		ret += 'q';
	if (chan.getr())
		ret += 'r';
	if (chan.gett())
		ret += 't';
	return ret + arg;
}

std::string Mode::receivModeIs(user &usr) {
	std::string ret = "+";
	if (usr.isAway())
		ret += 'a';
	if (usr.isRestrict())
		ret += 'r';
	if (usr.isInvisible())
		ret += 'i';
	if (usr.wallOp())
		ret += 'w';
	if (usr.isServOp())
		ret += 'o';
	return ret;
}

/*----------------------------------------------------------------------------*/

void	Mode::anonMode(bool onOff, Channel & chan) const
{
	std::string replies;
	std::string msg;
	std::set<user *>::iterator it;
	std::set<user *>::iterator it2;

	for(it = chan.getUsers().begin(); it != chan.getUsers().end(); ++it)
	{
		if (onOff == true)
		{
				replies = ":" + (*it)->getNickname() + " PART " + chan.getName() + " Anonymous_mode\r\n";
				if (send((*it)->getSock(), replies.c_str(), replies.length(), 0) == -1)
					std::cerr << strerror(errno) << std::endl;
		}
		if (onOff == false)
		{
			for(it2 = _serv->channels[chan.getName()]->getUsers().begin(); it2 != _serv->channels[chan.getName()]->getUsers().end(); ++it2)
			{
				msg = ":" + (*it)->getNickname() + " JOIN :" + chan.getName() + "\r\n";
				send((*it2)->getSock(), msg.c_str(), msg.length(), 0);
			}
			msg = ":" + (*it)->getNickname() + " JOIN :" + chan.getName() + "\r\n";
			send((*it)->getSock(), msg.c_str(), msg.length(), 0);
			_serv->getChannel(chan.getName())->send_names_replies((*it));
			_serv->send_replies((*it), chan.getName() + " :End of names list", RPL_ENDOFNAMES);
		}
	}
}