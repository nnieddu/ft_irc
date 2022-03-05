#pragma once

#include <vector>
#include <map>

#include "Socket.hpp"

class user
{
	private:
		user();
		user(const user & x);
		user& operator=(const user& x);

		std::string		_nickname;
		std::string		_username;
		std::string		_hostname;
		std::string		_afkMessage;

		bool			_server_operator;
		bool			_afk;
		bool			_restricted;
		bool			_invisible;
		bool			_wallOp;
		bool			_isLogged;
		Socket			_socket;

		std::map<std::string, unsigned int>	_channels;

	public:
		std::string	buf;

		user(const std::string&, const std::string&, const std::string&, const Socket&, bool);
		virtual ~user();

		void				setNickname(std::string nick);
		void				setLogged(bool log);

		bool				isServOp() const;
		void				promoteServ();
		void				demoteServ();

		void				join_channel(std::string&, bool);
		void				leave_channel(const std::string&);
		void				promote(const std::string&);
		void				demote(const std::string&);
		bool				isOperator(const std::string&) const;
		bool				isMember(const std::string&) const;

		bool				isVoice(const std::string&) const;
		void				setVoice(const std::string&);
		void				delVoice(const std::string&);

		bool				isRestrict() const;
		void				setRestrict();
		void				delRestrict();

		bool				isInvisible() const;
		void				setInvisible();
		void				delInvisible();

		bool				wallOp() const;
		void				setWallOp();
		void				delWallOp();

		bool				isAway() const;
		void				setAfk(const std::string&);
		void				delAfk();
		const std::string&	getAfkString() const;

		std::string							getNickname() const;
		std::string							getUsername() const;
		std::string							getHostname() const;
		std::string							getPrefix() const;
		Socket								getSocket() const;
		int									getSock() const;
		bool								getisLogged() const;
		std::map<std::string, unsigned int>	getChannels() const;
};
