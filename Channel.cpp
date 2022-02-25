#include <map>
#include <set>
#include <vector>
#include <utility>
#include <string>
#include "Channel.hpp"
#include "User.hpp"
#include "Socket.hpp"

channel::channel
	(const std::string & name, const std::string & password,
	 const server & serv, const user & owner)
	:_name(name), _password(password), _serv(&serv), _owner(&owner)
{
	_members.insert(std::make_pair(_owner->getNickname(), _owner));
	_operators.insert(_owner->getNickname());
}

channel::~channel() {}

void	channel::addUser(const user & new_user)
{
	_members.insert(std::make_pair(new_user.getNickname(), &new_user));
}

void	channel::removeUser(const std::string & username)
{
	_members.erase(username);
	_operators.erase(username);
//	if (_members.empty())
//		serv->close_channel(*this);
}

void	channel::UpgradePermissions(const std::string & username)
{
	if (isMember(username))
		_operators.insert(username);
}

void	channel::DowngradePermissions(const std::string & username)
{
	_operators.erase(username);
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

void	channel::SendToOne(const user & expeditor, const std::string & str, const std::string & username) const
{
	const std::string	message(expeditor.getUsername() + ": " + str);

	if (isMember(username) && expeditor.getNickname() != username)
		send(_members.find(username)->second->getSock(), message.c_str(), message.length(), 0);
}

bool	channel::isMember(const std::string & username) const
{
	return _members.find(username) != _members.end();
}


bool	channel::isOperator(const std::string & username) const
{
	return _operators.find(username) != _operators.end();
}

void	setName(const std::string & name);
