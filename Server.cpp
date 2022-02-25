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
			if (i == 0)
			{
				Socket	new_socket;
				try
				{
					new_socket.user_socket(_socket.fd);
				}
				catch(const std::runtime_error& e)
				{
					std::cerr << e.what() << " fail" << std::endl;
					end_server = 1;
				}

				std::cout << "New incoming connection - " << new_socket.fd << std::endl;
				std::stringstream	ss;

				ss << "nickname" << new_socket.fd;
				user	new_user(ss.str(), "username", "password", false, new_socket); //editer quand on saura quoi mettre la

				_users.push_back(new_user);
				_fds[_users.size()].fd = new_socket.fd;
				_fds[_users.size()].events = POLLIN;
			}
			else if ((ret_val = recv(_fds[i].fd, buf, sizeof(buf), 0)) >= 0)
			{
				if (ret_val < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << "recv() failed" << std::endl;
						close_user(i);
					}
					break;
				}
				else if (ret_val != 0)
				{
					std::cout << "Descriptor " << _fds[i].fd << " send : "<<  ret_val << " bytes :"<< std::endl;
					_cmds.isCommands(buf);
					_users[i - 1].buf += buf;

					memset(&buf, 0, sizeof(buf));
				}
				else
					close_user(i);
			}
		}
	}
	return(ret_val);
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
}
