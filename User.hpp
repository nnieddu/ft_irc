
#pragma once

#include <vector>
#include <set>

#include "Socket.hpp"

class user
{
	private:
		std::string				_ip;
		std::string				_nickname;
		std::string				_username;
		std::string				_password;
		bool					_isOperator;

		Socket					_socket;

		std::set<std::string>	_channels;
		std::string				_location;

		user();
		user(const user & x);
		user& operator=(const user& x);

	public:
		user(std::string ip, std::string nickname, std::string username, std::string password, 
			bool isOperator, const Socket & socket);
		virtual ~user();

		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getPassword() const;
		bool		IsOperator() const;
		Socket		getSocket() const;
		int			getSock() const;

		void		setOperator(bool opt);

		void		join_channel(std::string & name);
		void		leave_channel(std::string & name);
		void		setLocation(std::string & name);
		std::string	getLocation() const;		// /!\ locations related stuff
		bool		isMember(std::string & name) const;

		std::string	buf;
};
