
#pragma once

#include <iostream>
#include <cstring>
#include <stdexcept>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

class Socket
{

	public:
		int					fd;
		struct sockaddr_in	address;
		socklen_t			len;

		Socket();
		Socket(const Socket&);
		Socket& operator=(const Socket&);
		Socket(const int xfd, const struct sockaddr_in&, const socklen_t&);
		virtual	~Socket();

		void	server_socket(int);
};
