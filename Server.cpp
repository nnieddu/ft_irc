#include <string>
#include <sstream>

#include "Server.hpp"
#include "User.hpp"

server::server(const int & port, const std::string & password)
: _name("ft_irc.serv"), _port(port), _password(password)
{
	if (_port <= 1023 || _port > 65535)
		throw(std::invalid_argument(std::string("port number")));
	if (_password.size() < 5 || _password.size() > 16)
		throw(std::invalid_argument(std::string("password")));
}

server::~server() 
{
	for (size_t i = 0; i < _users.size() + 1; i++)
	{
		if(_fds[i].fd >= 0)
			close(_fds[i].fd);
	}
}

int 		server::getSock() const { return _socket.fd; }
std::string server::getName() const { return _name; }

int	server::run()
{
	int		ret_val(1);
	
	_socket.server_socket(_port);
	char	buf[80];

	memset(&buf, 0, sizeof(buf));
	memset(_fds, 0 , sizeof(_fds));

	// Set up the initial listening socket :
	_fds[0].fd = _socket.fd;
	_fds[0].events = POLLIN;
	
	std::cout << "Welcome on the irc server !" << std::endl
			<< "Waiting for connection..." << std::endl;

	size_t	i;
  	int		end_server = 0;
	
	while (end_server == 0)
  	{
		ret_val = poll(_fds, _users.size() + 1, -1); 
		if (ret_val < 0)
			throw(std::runtime_error("poll() failed"));

		for (i = 0; i <= _users.size(); i++)
		{
			if(_fds[i].revents == 0)
				continue;
			if(_fds[i].revents != POLLIN && _fds[i].revents != 25)
			{
				std::cerr << "Error! revents = " << _fds[i].revents << std::endl;
				end_server = 1;
				break;
			}
			else if (i == 0)
				end_server = accept_user();
			else
				receive_command(recv(_fds[i].fd, buf, sizeof(buf), 0), i, buf);
		}
	}
	return(ret_val);
}

int	server::accept_user()
{
	int					nfd;
	struct sockaddr_in	address;
	socklen_t			len = 0;
	std::stringstream	ss;

	try
	{
		nfd = accept(_socket.fd, reinterpret_cast<sockaddr*>(&address), &len);
		if (nfd == -1)
			throw(std::runtime_error("accept"));
		if (fcntl(nfd, F_SETFL, O_NONBLOCK) < 0)
			throw(std::runtime_error("fcntl()"));
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << " error" << std::endl;
		return 1;
	}

	ss << "nickname" << nfd;
	std::cout << "New incoming connection - " << ss.str() << std::endl;


	_users.push_back(user(ss.str(), "username", "password", false, Socket(nfd, address, len)));
	_fds[_users.size()].fd = nfd;
	_fds[_users.size()].events = POLLIN;
	return 0;
}

void	server::receive_command(ssize_t recv, size_t i, char *buf)
{
	if (recv < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			std::cerr << "recv() failed" << std::endl;
			close_user(i);
		}
	}
	else if (recv != 0)
	{
		std::cout << "Descriptor " << _fds[i].fd << " send : "<<  recv << " bytes :"<< std::endl;
		_cmds.launch(buf);
		_users[i - 1].buf += buf;

		memset(&buf, 0, sizeof(buf));
	}
	else
		close_user(i);
	return;
}

void	server::close_user(size_t i)
{
	std::cout << _users[i - 1].getNickname() << " deconnexion" << std::endl;
	close(_fds[i].fd);
	for (size_t j = i; j <= _users.size(); j++)
	{
		if (j < _users.size())
		{
			_fds[j].fd = _fds[j + 1].fd;
			_fds[j].events = _fds[j + 1].events;
			_fds[j].revents = _fds[j + 1].revents;
		}
		else
			_fds[j].fd = -1;
	}
	_users.erase(_users.begin() + (i - 1));
	return ;
}
