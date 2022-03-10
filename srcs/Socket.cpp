#include "../incs/Socket.hpp"
#include <arpa/inet.h>

/*----------------------------------------------------------------------------*/

Socket::Socket(): fd(-1), address(sockaddr_in()), len(socklen_t())
{}

Socket::Socket(const Socket & x):fd(x.fd), address(x.address), len(x.len)
{}

Socket::Socket(const int xfd, const struct sockaddr_in & xaddress, const socklen_t & xlen)
	:fd(xfd), address(xaddress), len(xlen)
{}

Socket& Socket::operator=(const Socket& x)
{
	if (this != &x)
	{
		fd = x.fd;
		address = x.address;
		len = x.len;
	}
	return *this;
}

Socket::~Socket()
{}

/*----------------------------------------------------------------------------*/

std::string	Socket::server_socket(int port)
{
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw(std::runtime_error("socket"));

	int	on = 1;
	if (setsockopt(fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
		throw(std::runtime_error(std::string("setsockopt() failed")));

	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw(std::runtime_error(std::string("fcntl() failed")));

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
		throw(std::runtime_error(std::string("bind")));

	if (listen(fd, 10) < 0)
		throw(std::runtime_error("listen"));

	return (inet_ntoa(reinterpret_cast<sockaddr_in*>(&address)->sin_addr));
}
