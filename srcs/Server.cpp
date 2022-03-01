
#include "../incs/Server.hpp"

server::server(const int & port, const std::string & password)
: _name("127.0.0.1"), _port(port), _password(password), _interpret(this)
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
	for (size_t index = 0; index < _users.size() + 1; index++)
	{
		if(_fds[index].fd >= 0)
			close(_fds[index].fd);
	}
	for (std::vector<user*>::iterator it = _users.begin() ; it != _users.end(); ++it)
		delete *it;
	for (std::map<std::string, Channel*>::iterator itc = channels.begin() ; itc != channels.end(); ++itc)
		delete itc->second;
}

int 		server::getSock() const { return _socket.fd; }
std::string server::getName() const { return _name; }
std::string server::getPassword() const { return _password; }

bool		server::isIn(std::string nickname) const
{ 
	for (std::vector<user*>::const_iterator it = _users.begin() ; it != _users.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return true; 
	}
	return false;
}

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
		for (size_t index = 0; index < nfd; index++)
		{
			if(_fds[index].revents == POLLIN || _fds[index].revents == 25)
			{
				if (index == 0)
					accept_user();
				else
					receive_data(index);
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
	std::string			hostname;
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
	catch(const std::runtime_error& er)
	{
		std::cerr << er.what() << " error" << std::endl;
		return ;
	}

	nick << "Guest" << new_pollfd.fd;
	std::cout << "New incoming connection - " << nick.str()<< std::endl;
	hostname = inet_ntoa(reinterpret_cast<sockaddr_in*>(&address)->sin_addr);
	user *new_user = new user(hostname, nick.str(), "username", Socket(new_pollfd.fd, address, len), false);
	_users.push_back(new_user);
	_fds.push_back(new_pollfd);

}

void	server::receive_data(size_t index)
{
	char	buf[512];
	std::string tmp;
	int ret = 0;

	memset(&buf, 0, sizeof(buf));
	if((ret = recv(_fds[index].fd, buf, sizeof(buf), 0)) <= 0)
	{
		if (ret < 0)
			std::cerr << "recv() failed" << std::endl;
		close_user(index);
		return ;
	}

	if (ret > 512) 
	{
		std::cerr << "Too long message" << std::endl; // a test et voir si une replies vas bien
		_users[index - 1]->buf.clear();
		return ;
	}

	tmp = buf;
	_users[index - 1]->buf += tmp;
	if (tmp.find("\n") != std::string::npos) // check si sur mac / a lecole netcat renvoi aussi un \r
	{
		std::cout << _users[index - 1]->getNickname() << " send : [" <<  _users[index - 1]->buf << "]" << std::endl;

		ret = _interpret.launch(*_users[index - 1]);
		_users[index - 1]->buf.clear();
		if (_users[index - 1]->getisLogged() == false)
		{
			//send replie // need to be log
			close_user(index);
		}
	}
}

void	server::close_user(size_t index)
{
	std::vector<user*>::iterator it = (_users.begin() + (index - 1));

	remove_user_from_channels(*it);
	std::cout << (*it)->getNickname() << " deconnexion" << std::endl; //to remove
	delete *it;
	close(_fds[index].fd);
	_fds.erase(_fds.begin() + index);
	_users.erase(_users.begin() + (index - 1));
	return ;
}

void	server::remove_user_from(user * usr, const std::string & name)
{
	if (channels[name]->getUsers().find(usr) != channels[name]->getUsers().end())
		channels[name]->removeUser(*usr);
}

void	server::remove_user_from_channels(user * usr)
{
	while (!(usr->getChannels().empty()))
	{
		std::string name = usr->getChannels().begin()->first;
		std::cout << usr->getNickname() << " leaving channel : " << name << std::endl;
		remove_user_from(usr, name);
		usr->leave_channel(name);
		if (channels[name]->getUsers().empty())		// <-- remove channel if its users vector is empty
			channels.erase(name);
	}
}

void	server::create_channel(user & usr, std::string & name)
{
	channels[name] = new Channel(usr, name);
	usr.join_channel(name, true);

	std::string replies = ":" + usr.getNickname() + " JOIN :" + name + "\r\n";
	send(usr.getSock(), replies.c_str(), replies.length(), 0);
}

void	server::send_replies(user *usr, std::string msg, const char* code)
{
	std::string replies;
	std::string prefix = ":" + usr->getUsername(); // usr->getUsername();
	msg += prefix;

	replies += (prefix +  " " + code + " " + usr->getNickname() + " " + msg + "\r\n");
	send(usr->getSock(), replies.c_str(), replies.length(), 0);
}

void	server::send_to_chan(user * usr, std::string chan)
{
	std::string msg;
	std::set<user *>::iterator it;
	channels[chan]->addUser(*usr);
	for(it = channels[chan]->getUsers().begin(); it != channels[chan]->getUsers().end(); ++it)
	{
		msg = ":" + (*it)->getNickname() + " JOIN :" + chan + "\r\n";
		std::cout << "sendtochan=[" << msg << "]" << std::endl;
	  	send((*it)->getSock(), msg.c_str(), msg.length(), 0);
	}
}
