#include <map>
#include <set>
#include <vector>
#include <utility>
#include <string>
#include "Channel.hpp"
#include "User.hpp"
#include "Socket.hpp"
#include "Server.hpp"

channel::channel(const channel& x):_name(x.getName())
{
	_members.insert(x.getMembers().begin(), x.getMembers().end());
}

channel::channel(std::string & name, const user & creator)
	:_name(name)
{
	_members.insert(std::make_pair(creator.getNickname(), &creator));
}

channel::~channel() {}

void	channel::addUser(const user & usr)
{
	_members.insert(std::make_pair(usr.getNickname(), &usr));
}

void	channel::removeUser(const user & usr)
{
	_members.erase(usr.getNickname());
}

void	channel::SendToAll(const user & expeditor, const std::string & str) const
{
	std::map<std::string, const user*>::const_iterator	it(_members.begin());
	const user*											current_user;
	const std::string									message(expeditor.getUsername() + ": " + str);

	while (it != _members.end())
	{
		current_user = it->second;
		if (current_user->getNickname() != expeditor.getNickname())
			send(current_user->getSock(), message.c_str(), message.length(), 0);
		it++;
	}
}

bool	channel::isMember(const user & usr) const
{
	return _members.find(usr.getNickname()) != _members.end();
}

std::string	channel::getName() const
{
	return _name;
}

void	channel::setName(const std::string & new_name)
{
	_name = new_name;
}

std::map<std::string, const user*>	channel::getMembers() const
{
	return _members;
}

bool	channel::empty() const
{
	return _members.empty();
}
