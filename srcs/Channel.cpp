//
// Created by fenrir on 27/02/2022.
//

#include "../incs/Channel.hpp"

Channel::Channel(user &users, std::string &newName) : users(), name(), id(),
topic(), password("password"), limit_user(0), chanCrea(NULL), chanOps() {
	this->users.push_back(&users);
	id.clear();
	if (newName[0] == '&' || newName[0] == '#') {
		mod = o;
		chanOps.push_back(&users);
	}
	else if (newName[0] == '+') {
		mod = t;
	}
	else if (newName[0] == '!' && newName[1] == '!') {
		mod = O + o;
		chanOps.push_back(&users);
		chanCrea = &users;

		srand(time(NULL));
		for (int index = 0; index < 5; index++) {
			int random = rand() % 62;
			if (random < 10)
				id.push_back(static_cast<char>(random + 48));
			else if (random < 36)
				id.push_back(static_cast<char>(random + 55));
			else
				id.push_back(static_cast<char>(random + 61));
		}
		name = newName.erase(0, 1);

	}
	else
		throw std::exception(); //TODO:: need to be modified
}

void Channel::setMod(unsigned int newMod) {
	mod = newMod ^ mod;
}

void Channel::setLim(unsigned int newLim) {
	limit_user = newLim;
}

void Channel::setTopic(const std::string &newTopic) {
	topic = newTopic;
}

void Channel::setPass(const std::string &newPass) {
	password = newPass;
}

void Channel::addOp(user &newOp) {
	chanOps.push_back(&newOp);
}

void Channel::addUser(user &newUser) {
	users.push_back(&newUser);
}

const unsigned int &Channel::getMod() const {
	return mod;
}

const unsigned int &Channel::getLim() const {
	return limit_user;
}

const std::string &Channel::getName() const {
	return name;
}

const std::string &Channel::getId() const {
	return id;
}

const std::string &Channel::getTopic() const {
	return topic;
}

const std::string &Channel::getPass() const {
	return password;
}

const user &Channel::getChanCrea() const {
	return *chanCrea;
}

const std::vector<user *> &Channel::getChanOps() const {
	return chanOps;
}

const std::vector<user *> &Channel::getUsers() const {
	return users;
}
