#include <string>
#include <sstream>

#include "Server.hpp"
#include "User.hpp"
#include "irc_replies.hpp"

server::server(const int & port, const std::string & password)
: _name("ft_irc.serv"), _port(port), _password(password), _cmds(this)
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

void	server::send_reply(user *usr, const char* code)
{
	std::string to_send;
	std::string prefix = ":" + usr->getUsername();
	std::string test = "Welcome to the Internet Relay Network " + prefix;

	to_send += (prefix +  " " + code + " " + usr->getNickname() + " " + test + "\r\n");
	send(usr->getSock(), to_send.c_str(), to_send.length(), 0);
}

int	server::run()
{
	int										ret_val(1);
	struct pollfd							serv_fd;
	std::vector<struct pollfd>::iterator	it;
	char									buf[80];

	_socket.server_socket(_port);

	memset(&buf, 0, sizeof(buf));

	// Set up the initial listening socket :
	serv_fd.fd = _socket.fd;
	serv_fd.events = POLLIN;

	_fds.push_back(serv_fd);
	
	std::cout << "Welcome on the irc server !" << std::endl
			<< "Waiting for connection..." << std::endl;
	
	for (int end_server = 0; end_server == 0;)
  	{
		it = _fds.begin();
		ret_val = poll(&(*it), _fds.size(), -1); 
		if (ret_val < 0)
			throw(std::runtime_error("poll() failed"));

		size_t nfd = _fds.size();
		for (size_t i = 0; i < nfd; i++)
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
	struct pollfd		new_pollfd;
	struct sockaddr_in	address;
	socklen_t			len = 0;
	std::stringstream	ss;

	new_pollfd.fd = -1;
	new_pollfd.events = POLLIN;
	try
	{
		int	nfd;

		nfd = accept(_socket.fd, reinterpret_cast<sockaddr*>(&address), &len);
		if (nfd == -1)
			throw(std::runtime_error("accept"));
		if (fcntl(nfd, F_SETFL, O_NONBLOCK) < 0)
			throw(std::runtime_error("fcntl()"));
		new_pollfd.fd = nfd;
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << " error" << std::endl;
		return 1;
	}

	ss << "nickname" << new_pollfd.fd;
	std::cout << "New incoming connection - " << ss.str() << std::endl;


	_users.push_back(user(ss.str(), "username", "password", false, Socket(new_pollfd.fd, address, len)));
	_fds.push_back(new_pollfd);
	send_reply(&_users[_users.size() - 1], RPL_WELCOME);
    // send_reply(&_users[_users.size() - 1], RPL_YOURHOST);
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
		_users[i - 1].buf += buf;
		_cmds.launch(_users[i - 1]);

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
	_fds.erase(_fds.begin() + i);
	_users.erase(_users.begin() + (i - 1));
	return ;
}
