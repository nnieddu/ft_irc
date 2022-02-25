
#pragma once

#include <iostream>
#include <string>
#include <set>
#include "Socket.hpp"

class user
{
	private:

		user();
		
		std::string	_nickname;
		std::string	_username;
		std::string	_password;
		bool		_isOperator;
		Socket		_socket;

		std::set<std::string>	_channels;
		std::string				_location;

	public:

		user(std::string nickname, std::string username, std::string password, bool isOperator, const Socket & socket);
		user(const user & x);
		user& operator=(const user& x);
		virtual ~user();

		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getPassword() const;
		bool		IsOperator() const;
		Socket		getSocket() const;

		int			getSock() const;

		void		join_channel(std::string & name);
		void		leave_channel(std::string & name);
		void		setLocation(std::string & name);
		std::string	getLocation() const;		// /!\ locations related stuff
		bool		isMember(std::string & name) const;

		std::string	buf;
		void	clearbuf();
};

std::ostream& operator<<(std::ostream& os, const user& user);