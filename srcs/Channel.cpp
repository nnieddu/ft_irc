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

/*----------------------------------------------------------------------------*/

Channel::Channel(user &users, std::string &newName) : users(), users_invited(), id(),
name(nameCaseIns(newName)), topic(), password("password"), limit_user(0),
chanCrea(NULL), rtime(0), mod(n  + t) {
	this->users.insert(&users);
	id.clear();
	topic.clear();
	if (newName[0] == '!') {
		chanCrea = &users;

		srand(time(NULL));
		for (int index = 0; index < 5; index++) {
			int random = rand() % 36;
			if (random < 10)
				id.push_back(static_cast<char>(random + 48));
			else
				id.push_back(static_cast<char>(random + 87));
		}
	}
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

Channel::~Channel(){}

/*----------------------------------------------------------------------------*/

void Channel::setMod(unsigned int newMod) {
	mod = newMod | mod;
}

void Channel::removeMod(unsigned int oldMod) {
	mod = mod & ~oldMod;
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

void Channel::removeUser(user &remUser){
	users.erase(users.find(&remUser));
}

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

bool Channel::isInvited(user& usr) const
{
	if (users_invited.find(&usr) != users_invited.end() && !users_invited.empty())
		return true;
	return false ;
}

void Channel::addInvited(user &newUser) { users_invited.insert(&newUser); }

void Channel::remInvited(user &remUser) { users_invited.erase(users_invited.find(&remUser)); }

/*----------------------------------------------------------------------------*/

void	Channel::send_names_replies(const user * receiver) const
{
	std::string usersInChan;
	std::set<user *>::iterator it;

	for(it = this->getUsers().begin(); it != this->getUsers().end(); ++it)
	{
		if (!this->geta())
		{
			if ((*it)->isOperator(name))
				usersInChan += "@";
			usersInChan += (*it)->getNickname() + " ";
		}
	}
	std::string replies = ":" + receiver->getUsername() +  " " + RPL_NAMREPLY + " " \
		+ receiver->getNickname() + " " +  "= " + name + " :" + usersInChan + "\r\n";

	send(receiver->getSock(), replies.c_str(), replies.length(), 0);
}

/*----------------------------------------------------------------------------*/

bool	Channel::hasOp(const std::time_t & now)
{
	for (std::set<user*>::iterator it(users.begin()); it != users.end(); it++)
	{
		if ((*it)->isOperator(name))
		{
			rtime = 0;
			return true;
		}
	}
	if (rtime == 0)
		rtime = now;
	return false;
}

bool	Channel::mustAddOp(const std::time_t & now)
{
	return hasOp(now) == false && difftime(now, rtime) > RFLAG_TIME_SEC;
}

void	Channel::selectRandomOp(const std::string & servname)
{
	std::set<user*>::iterator	it(users.begin());
	std::string					reply;

	for (size_t	index(std::rand() % users.size()); index > 0; index--)
		it++;

	reply = ":" + servname + " MODE " + name + " +o :" + (*it)->getNickname() + "\n\r";
	(*it)->promote(name);

	for (it = users.begin(); it != users.end(); it++)
		send((*it)->getSock(), reply.c_str(), reply.size(), 0);
	return;
}

/*----------------------------------------------------------------------------*/

bool Channel::geta() const {
	if (mod & a)
		return true;
	return false;
}

bool Channel::geti() const {
	if (mod & i)
		return true;
	return false;
}

bool Channel::getm() const {
	if (mod & m)
		return true;
	return false;
}

bool Channel::getn() const {
	if (mod & n)
		return true;
	return false;
}

bool Channel::getq() const {
	if (mod & q)
		return true;
	return false;
}

bool Channel::getp() const {
	if (mod & p)
		return true;
	return false;
}

bool Channel::getr() const {
	if (mod & r)
		return true;
	return false;
}

bool Channel::gett() const {
	if (mod & t)
		return true;
	return false;
}

bool Channel::getk() const {
	if (mod & k)
		return true;
	return false;
}

bool Channel::getl() const {
	if (mod & l)
		return true;
	return false;
}
