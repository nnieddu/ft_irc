/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrozniec <mrozniec@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 13:56:40 by mrozniec          #+#    #+#             */
/*   Updated: 2022/03/02 20:30:55 by mrozniec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"
#include "../incs/Server.hpp"

Channel::Channel(user &users, std::string &newName) : users(), name(nameCaseIns(newName)), id(),
topic("No topic set for channel "), password("password"), limit_user(0), chanCrea(NULL) {
	this->users.insert(&users);
	id.clear();
	if (newName[0] == '&' || newName[0] == '#')
		mod = o;
	else if (newName[0] == '+')
		mod = t;
	else if (newName[0] == '!' && newName[1] == '!') {
		mod = O + o;
		chanCrea = &users;

		srand(time(NULL));
		for (int index = 0; index < 5; index++) {
			int random = rand() % 36;
			if (random < 10)
				id.push_back(static_cast<char>(random + 48));
			else
				id.push_back(static_cast<char>(random + 87));
		}
		name.erase(0, 1);
	}
	topic = topic + name[0] + id + name.substr(1);
}

void Channel::setMod(unsigned int newMod) {
	mod = newMod | mod;
}

void Channel::removeMod(unsigned int oldMod) {
	mod = mod & ~oldMod;
}

void Channel::setBanMask(const std::string &newMask) {
	banMask = newMask;
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

void Channel::addUser(user &newUser) {
	users.insert(&newUser);
}

void Channel::removeUser(user &remUser) {
	users.erase(users.find(&remUser));
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

const std::set<user *> &Channel::getUsers() const {
	return users;
}

Channel &Channel::operator=(const Channel &old) {
	if (this != &old) {
		this->mod = old.mod;
		this->limit_user = old.limit_user;
		this->name = old.name;
		this->id = old.id;
		this->topic = old.topic;
		this->password = old.password;
		this->chanCrea = old.chanCrea;
		this->users = old.users;
	}
	return *this;
}
