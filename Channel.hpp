
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "User.hpp"

class server;

class channel
{
	private:

		channel();
		channel(const channel& x);
		channel& operator=(const channel& x);

		std::string	_name;

		const server*						_serv;
		const user*							_owner;

		std::map<std::string, const user*>	_members;
		std::set<std::string>				_operators;

	public:

		channel(const std::string & name, const server & serv, const user & creator);
		virtual ~channel();

		void	addUser(const user & new_user);
		void	removeUser(const std::string & username);
		void	UpgradePermissions(const std::string & username);
		void	DowngradePermissions(const std::string & username);

		void	SendToAll(const user & expeditor, const std::string & str) const;
		void	SendToOne(const user & expeditor, const std::string & str, const std::string & username) const;

		bool	isMember(const std::string & username) const;
		bool	isOperator(const std::string & username) const;

		void	setName(const std::string & name);

};
