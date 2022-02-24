
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <string>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#include <netinet/in.h> 
#include <sys/socket.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

#include "Socket.hpp"
#include "User.hpp"
// #include "Channel.hpp"

class server
{
	private:

		server();
		server(const server& x);
		server& operator=(const server& x);

		std::string			_name;
		int					_port;
		std::string			_password;
		int					_nfds; //nbr de fds
		Socket				_socket;
		struct pollfd 		_fds[200];

		std::map<std::string, user>	_users;
//		std::map<int, channel>	_channels;

	public:

		server(const int & port, const std::string & password);
		~server();
		
		int	getSock() const;
		std::string	getName() const;

		int	start();
};
