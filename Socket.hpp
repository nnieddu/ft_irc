
#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>

class Socket
{

	private:

		Socket(const Socket&);
		Socket& operator=(const Socket& x);

	public:

		int					fd;
		struct sockaddr_in	address;
		socklen_t			len;

		Socket();
		virtual	~Socket();

		void	init(int port);
};
