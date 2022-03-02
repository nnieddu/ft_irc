/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrozniec <mrozniec@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 13:56:40 by mrozniec          #+#    #+#             */
/*   Updated: 2022/03/02 16:57:13 by mrozniec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Channel.hpp"

Channel::Channel(user &users, std::string &newName) : users(), name(), id(), banMask(),
topic("No topic set for channel "), password("password"), limit_user(0), chanCrea(NULL) {
	this->users.insert(&users);
	id.clear();
	banMask.clear();
	//channel name are case-insensitive
//	for (int index = 0; newName[index]; index++)
//		name.push_back(static_cast<char>(std::tolower(newName[index])));
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
		name = newName.substr(1);
	}
	topic = topic + name[0] + id + name.substr(1);
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

void Channel::setBanMask(const std::string &newMask) {
	this->banMask = newMask;
}

const std::string &Channel::getBanMask() const {
	return this->banMask;
}
