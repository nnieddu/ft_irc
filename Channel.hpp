
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
		channel& operator=(const channel& x);

		std::string	_name;

		std::map<std::string, const user*>	_members;

	public:

		channel(const channel& x);
		channel(std::string & name, const user & creator);
		virtual ~channel();

		void	addUser(const user & usr);
		void	removeUser(const user & usr);

		void	SendToAll(const user & expeditor, const std::string & str) const;

		bool	isMember(const user & usr) const;

		std::string	getName() const;
		void	setName(const std::string & new_name);

		std::map<std::string, const user*>	getMembers() const;

		bool	empty() const;

};
