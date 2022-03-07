
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
#include <ctime>
#include <sstream>

#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Socket.hpp"
#include "User.hpp"
#include "Interpret.hpp"
#include "NumericReplies.hpp"
#include "Channel.hpp"

#define	REFRESH_TIME_MS	1000
#define INACTIVE_SEC	15

class server
{
	public:
		class quitexcept : public std::exception
		{
			public:

				virtual const char *	what() const throw()
				{ return "Closing server..."; }
		};

	private:
		server();
		server(const server& x);
		server& operator=(const server& x);

		std::string									_name;
		int											_port;
		std::string									_password;
		Socket										_socket;
		Interpret									_interpret;
		std::vector<struct pollfd> 					_fds;
		std::vector<user*>							_users;

	public:
		std::map<std::string, Channel* > channels;

		server(const int & port, const std::string & password);
		~server();

		void	run();

		void	accept_user();
		void	close_user(size_t);

		void	receive_data(size_t);

		void	remove_user_from(user*, const std::string&, const std::string&);
		void	remove_user_from_channels(user*, const std::string&);

		void	send_replies(user*, const std::string&, const char*) const;
		int		send_msg_to_user(const user*, const user*, const std::string&, const std::string&) const;
		int		send_msg_to_channel(const user*, const Channel*, const std::string&) const;

		void	ping(user *, int);
		void	pong(const std::string&);

		bool 	isUser(const std::string&) const;

		std::string			getName() const;
		int					getPort() const;
		std::string			getPassword() const;
		std::vector<user*>	getUsers() const;
		int					getSock() const;
		user *				getUser(const std::string&) const;
		Channel *			getChannel(const std::string&) const;
		void				welcomeNewUser(user * usr);

};

std::string&	nameCaseIns(std::string&);
std::string 	ft_itoa(int nbr);