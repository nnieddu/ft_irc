#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include "Socket.hpp"

Socket::Socket(): fd(0)
{}

Socket::Socket(const int & port): port(port), fd(0)
{}

Socket::~Socket()
{
	if (fd)
		close(fd);
}

void	Socket::init()
{
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (fd == -1)
		throw(std::runtime_error("socket"));

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == -1)
		throw(std::runtime_error("bind"));

	if (listen(fd, 5))
		throw(std::runtime_error("listen"));

	return;
}
