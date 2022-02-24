
#pragma once

#include <iostream>
#include <string>
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

		std::string	buf;
};
