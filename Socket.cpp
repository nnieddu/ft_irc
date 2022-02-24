
#include <iostream>
#include <cstring>
#include <stdexcept>

#include <unistd.h>

#include "Socket.hpp"

Socket::Socket(): fd(-1)
{}

Socket::~Socket()
{
	if (fd)
		close(fd);
}

void	Socket::init(int port)
{
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw(std::runtime_error("socket"));

	// /* MAYBE USELESS AFTER TESTING (permet de restart le server rapidement sans blockage)
 	// // Allow server socket descriptor to be reuseable
	// // SOL_SOCKET = set options at the socket level
	// // SO_REUSEADDR = allow reuse of local addresses */
	int	on = 1;
	if (setsockopt(fd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
		throw(std::runtime_error(std::string("setsockopt() failed")));

	// // Passe le fd/socket en mode non bloquant (pourrait etre fais a la creation du socket avec 
	// // socket(O_NONBLOCK)) mais d'apres le man, ne fonctionne que sous Linux et usage de fcntl standardise par Posix.
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw(std::runtime_error(std::string("fcntl() failed")));

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw(std::runtime_error(std::string("bind")));

	if (listen(fd, 10) < 0)
		throw(std::runtime_error("listen"));

	return;
}
