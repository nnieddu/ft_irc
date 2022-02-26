
#include "../incs/Server.hpp"

server::server(const int & port, const std::string & password)
: _name("ft_irc.serv"), _port(port), _password(password), _cmds(this)
{
	if (_port <= 1023 || _port > 65535)
		throw(std::invalid_argument(std::string("port number")));
	if (_password.size() < 5 || _password.size() > 16)
		throw(std::invalid_argument(std::string("password")));

	_socket.server_socket(_port);

	struct pollfd	serv_fd;
	memset(&serv_fd, 0, sizeof(serv_fd));
	serv_fd.fd = _socket.fd;
	serv_fd.events = POLLIN;

	_fds.push_back(serv_fd);
}

server::~server() 
{
	for (size_t i = 0; i < _users.size() + 1; i++)
	{
		if(_fds[i].fd >= 0)
			close(_fds[i].fd);
	}
	for (std::vector<user*>::iterator it = _users.begin() ; it != _users.end(); ++it)
		delete *it;
}

int 		server::getSock() const { return _socket.fd; }
std::string server::getName() const { return _name; }
std::string server::getPassword() const { return _password; }


static void	ft_exit(int sign)
{
	if (sign == 2)
		std:: cout << std::endl << "SIGINT" << std::endl;
	else
		std:: cout << std::endl << "SIGQUIT" << std::endl;
	throw(server::quitexcept());
}

int	server::run()
{
	signal(SIGINT, ft_exit);
	signal(SIGQUIT, ft_exit);

	std::vector<struct pollfd>::iterator	it;

	for (;;)
	{
		it = _fds.begin();
		if (poll(&(*it), _fds.size(), -1) < 0)
			throw(std::runtime_error("poll() failed"));

		size_t nfd = _fds.size();
		for (size_t i = 0; i < nfd; i++)
		{
			if(_fds[i].revents == POLLIN || _fds[i].revents == 25)
			{
				if (i == 0)
					accept_user();
				else
					receive_data(i);
			}								// /!\ revent_error ?
		}
	}
	return(0);
}

void server::accept_user()
{
	struct pollfd		new_pollfd;
	sockaddr_in			address;
	socklen_t			len = sizeof(sockaddr);
	std::string			ip;
	std::stringstream	nick;

	memset(&new_pollfd, 0, sizeof(new_pollfd));
	new_pollfd.fd = -1;
	new_pollfd.events = POLLIN;
	try
	{
		if ((new_pollfd.fd = accept(_socket.fd, reinterpret_cast<sockaddr*>(&address), &len)) == -1)
			throw(std::runtime_error("accept"));
		if (fcntl(new_pollfd.fd, F_SETFL, O_NONBLOCK) < 0)
			throw(std::runtime_error("fcntl()"));
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << " error" << std::endl;
		return ;
	}

	nick << "nickname" << new_pollfd.fd;
	std::cout << "New incoming connection - " << nick.str()<< std::endl;
	ip = inet_ntoa(reinterpret_cast<sockaddr_in*>(&address)->sin_addr);
	user *new_user = new user(ip, nick.str(), "username", "password", Socket(new_pollfd.fd, address, len), false);
	_users.push_back(new_user);
	_fds.push_back(new_pollfd);

	send_replies(_users[_users.size() - 1], "Welcome to the Internet Relay Network ", RPL_WELCOME); 
}

void	server::receive_data(size_t i)
{
	char	buf[512];
	std::string tmp;
	int ret = 0;

	memset(&buf, 0, sizeof(buf));
	if((ret = recv(_fds[i].fd, buf, sizeof(buf), 0)) <= 0)
	{
		if (ret < 0)
			std::cerr << "recv() failed" << std::endl;
		close_user(i);
		return ;
	}
	tmp = buf;
	_users[i - 1]->buf += tmp;
	std::cout << tmp << std::endl;
	// if (tmp.find("\r") != std::string::npos) // weechat ok
	if (tmp.empty() == false) // trouver comment detecter fin de commande via nc
	{
		std::cout << _users[i - 1]->getNickname() << " send : " << std::endl << tmp; //
		_cmds.launch(*_users[i - 1]);
		_users[i - 1]->buf.clear();
	}
}

void	server::close_user(size_t i)
{
	std::vector<user*>::iterator it = (_users.begin() + (i - 1));

	remove_user_from_channels(*it);
	std::cout << (*it)->getNickname() << " deconnexion" << std::endl; //to remove
	delete *it;
	close(_fds[i].fd);
	_fds.erase(_fds.begin() + i);
	_users.erase(_users.begin() + (i - 1));
	return ;
}

void	server::remove_user_from(user * usr, const std::string & name)
{
	std::vector<user *>::iterator	it(_channels[name].begin());

	while ((*it)->getNickname() != usr->getNickname())
		it++;
	_channels[name].erase(it);
}

void	server::remove_user_from_channels(user * usr)
{
	while (!(usr->getChannels().empty()))
	{
		std::string name = usr->getChannels().begin()->first;

		std::cout << usr->getNickname() << " leaving channel : " << name << std::endl;
		remove_user_from(usr, name);
		usr->leave_channel(name);
		if (_channels[name].empty())		// <-- remove channel if its users vector is empty
			_channels.erase(name);
	}
}

void	server::create_channel(user & usr, std::string & name)
{
	_channels[name].push_back(&usr);
	usr.join_channel(name, true);
	
	std::string reply = ":" + usr.getNickname() + " JOIN :" + name + "\r\n";
	send(usr.getSock(), reply.c_str(), reply.length(), 0);
}

void	server::send_replies(user *usr, std::string msg, const char* code)
{
	std::string to_send;
	std::string prefix = ":" + usr->getUsername();
	msg += prefix;

	to_send += (prefix +  " " + code + " " + usr->getNickname() + " " + msg + "\r\n");
	send(usr->getSock(), to_send.c_str(), to_send.length(), 0);
}
