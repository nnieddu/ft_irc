
#pragma once

#include <iostream>
#include <string>
#include "Socket.hpp"

class user
{
	private:

		user();
		user(const user& x);
		user& operator=(const user& x);
		
		std::string	_name;
		std::string	_nickname;
		std::string	_password;
		Socket		_socket;
		bool		_isOperator;

	public:

		user(std::string name, std::string password, bool isOperator, int serverSocket);
		virtual ~user();

		std::string	getName() const;
		std::string	getNickname() const;
		int			getSock() const;
};