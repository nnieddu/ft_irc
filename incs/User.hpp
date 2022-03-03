
#pragma once

#include <vector>
#include <map>

#include "Socket.hpp"

class user
{
	private:
		std::string					_nickname;
		std::string					_username;
		std::string					_hostname;
		std::string					_password;

		bool						_server_operator;
		bool						_isLogged;
		Socket						_socket;

		std::map<std::string, unsigned int>	_channels;
		std::string					_location;

		user();
		user(const user & x);
		user& operator=(const user& x);

	public:
		user(std::string hostname, std::string nickname, std::string username,
			const Socket & socket, bool server_operator);
		virtual ~user();

		std::string	buf;

		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getPassword() const;
		std::string	getHostname() const;
		Socket		getSocket() const;
		int			getSock() const;
		bool		getisLogged() const;
		std::string	getLocation() const;
		std::map<std::string, unsigned int>	getChannels() const;

		void		setPassword(std::string psw);
		void		setNickname(std::string nick);
		void		setLogged(bool log);
		void		setLocation(std::string & name);

		bool		isOperator(const std::string & name) const;
		void		promote(const std::string & name);
		void		demote(const std::string & name);
		void		join_channel(const std::string & name, bool op);
		void		leave_channel(const std::string & name);
		bool		isMember(const std::string & name) const;

};
