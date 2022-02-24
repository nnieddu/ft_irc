
#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>

class Socket
{

	public:

		int					fd;
		struct sockaddr_in	address;
		socklen_t			len;

		Socket();
		Socket(const Socket & x);
		Socket& operator=(const Socket& x);
		virtual	~Socket();

		void	server_socket(int port);
		void	user_socket(const int & server);
};
