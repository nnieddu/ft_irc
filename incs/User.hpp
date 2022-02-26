
#pragma once

#include <vector>
#include <map>

#include "Socket.hpp"

class user
{
	private:
		std::string					_ip;
		std::string					_nickname;
		std::string					_username;
		std::string					_password;

		bool						_server_operator;
		Socket						_socket;

		std::map<std::string, bool>	_channels;
		std::string					_location;

		user();
		user(const user & x);
		user& operator=(const user& x);

	public:
		user(std::string ip, std::string nickname, std::string username, std::string password, 
			const Socket & socket, bool server_operator);
		virtual ~user();

		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getPassword() const;
		Socket		getSocket() const;
		int			getSock() const;

		std::map<std::string, bool>	getChannels() const;

		bool		isOperator(std::string & name) const;
		void		promote(std::string & name);
		void		demote(std::string & name);

		void		join_channel(std::string & name, bool op);
		void		leave_channel(std::string & name);
		void		setLocation(std::string & name);
		std::string	getLocation() const;		// /!\ locations related stuff
		bool		isMember(std::string & name) const;

		std::string	buf;
};