
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <sstream>

#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Socket.hpp"
#include "User.hpp"
#include "Commands.hpp"
#include "NumericReplies.hpp"

class server
{
	private:
		server();
		server(const server& x);
		server& operator=(const server& x);

		std::string									_name;
		int											_port;
		std::string									_password;
		Socket										_socket;
		Commands									_cmds;
		std::vector<struct pollfd> 					_fds;
		std::vector<user*>							_users;

	public:
		server(const int & port, const std::string & password);
		~server();
		
		std::map<std::string, std::vector<user*> > _channels;

		int			getSock() const;
		std::string	getName() const;

		class quitexcept : public std::exception
		{
			public:

				virtual const char *	what() const throw()
				{	return "Closing program...";}
		};

		int		run();
		void	accept_user();
		void	receive_data(size_t i);
		void	close_user(size_t i);
		void	remove_user_from(user * usr, const std::string & name);
		void	remove_user_from_channels(user * usr);
		void	send_replies(user *usr, std::string msg, const char* code);

		void 	exit(user & usr, std::string cmd);
		void	create_channel(user & usr, std::string & name);
};