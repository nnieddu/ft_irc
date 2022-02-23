
#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

class Socket
{

	private:

		Socket(const Socket&);
		Socket& operator=(const Socket& x);

	public:

		int					port;
		int					fd;
		struct sockaddr_in	address;
		socklen_t			len;

		Socket();
		Socket(const int & port);
		virtual	~Socket();

		void	init();
};
