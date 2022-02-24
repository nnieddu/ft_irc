
#include "Server.hpp"
#include "User.hpp"

server::server(const int & port, const std::string & password)
: _name("ft_irc.serv"), _port(port), _password(password), _nfds(1)
{
	if (_port <= 1023 || _port > 65535)
		throw(std::invalid_argument(std::string("port number")));
	if (_password.size() < 5 || _password.size() > 16)
		throw(std::invalid_argument(std::string("password")));
}

server::~server() 
{
	for (int i = 0; i < _nfds; i++)
	{
		if(_fds[i].fd >= 0)
			close(_fds[i].fd);
	}
}

int 		server::getSock() const { return _socket.fd; }
std::string server::getName() const { return _name; }

int	server::start()
{
	int		ret_val(1);
	char	buf[80];
	
	_socket.init(_port);
	
	memset(_fds, 0 , sizeof(_fds)); // Initialize the pollfd structure 
	// Set up the initial listening socket :
	_fds[0].fd = _socket.fd;
	_fds[0].events = POLLIN;
	
	std::cout << "Welcome on the irc server !" << std::endl
			<< "Waiting for connection..." << std::endl;

	int		i, j;
	int		current_size = 0;
	int		new_sd = -1;
  	int 	end_server = 0;
	int		compress_array = 0;
	int 	close_conn;
	
	while (end_server == 0)
  	{
		ret_val = poll(_fds, _nfds, (1 * 60 * 1000)); 
		// 3 * 60 * 1000 = 1 minutes : timeout si pas de co / data pdt 1 min // _nfds = nombre de fd dans la struct
		if (ret_val < 0)
			throw(std::runtime_error("poll() failed"));
		if (ret_val == 0) // == 0 if timeout
			throw(std::runtime_error("poll() timed out"));

		current_size = _nfds; 
		for (i = 0; i < current_size; i++)
		{
			if(_fds[i].revents == 0)
				continue;
			if(_fds[i].revents != POLLIN) //POLLIN = There is data to read.
			{
				std::cerr << "Error! revents = " << _fds[i].revents << std::endl;
				end_server = 1;
				break;
			}
			if (_fds[i].fd == _socket.fd)
			{
				std::cout << "Listening socket is readable" << std::endl;
				do
				{
					// map insert avec gethostname etc..
					// user	new_user("tmp", "tmp", false, getSock());
					new_sd = accept(_socket.fd, NULL, NULL);

					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "accept() failed" << std::endl;
							end_server = 1;
						}
						break;
					}
					std::cout << "New incoming connection - " << new_sd << std::endl;
					_fds[_nfds].fd = new_sd;
					_fds[_nfds].events = POLLIN;
					_nfds++;
				} while (new_sd != -1);
			}
			else if ((ret_val = recv(_fds[i].fd, buf, sizeof(buf), 0)) >= 0)
			{
				std::cout << "Descriptor " << _fds[i].fd << " send : "<<  ret_val << " bytes :"<< std::endl;
				close_conn = 0;
				if (ret_val < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << "recv() failed" << std::endl;
						close_conn = 1;
					}
					break;
				}
				if (ret_val == 0)
				{
					std::cout << "Connection closed" << std::endl;
					close_conn = 1;
				}
				//******************************* test exit cmd
				std::cout << buf << std::endl;
				if ((strcmp(buf, "/exit\n") == 0))
					return(0);
				//*******************************
				memset(&buf, 0, sizeof(buf)); // clear buffer pr pas avoir de la merde
				if (close_conn)
				{
					close(_fds[i].fd);
					_fds[i].fd = -1;
					compress_array = 1;
				}
			}
		}
		if (compress_array)
		{
			compress_array = 0;
			for (i = 0; i < _nfds; i++)
				if (_fds[i].fd == -1)
				{
					for(j = i; j < _nfds; j++)
						_fds[j].fd = _fds[j+1].fd;
					i--;
					_nfds--;
				}
		}
	}
	return(ret_val);
}
