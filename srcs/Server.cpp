#include "../incs/Server.hpp"

/*----------------------------------------------------------------------------*/

server::server(const int & port, const std::string & strport, const std::string & password)
: _name("127.0.0.1"), _port(port), _strport(strport), _password(password), _interpret(this)
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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

static void	ft_exit(int sign)
{
	static int FLAG = 0;

	if (FLAG != 0)
		throw(server::quitexcept());
	FLAG = sign;
	if (sign == 2)
		std:: cout << std::endl << "SIGINT" << std::endl;
	else if (sign != 0)
		std:: cout << std::endl << "SIGQUIT" << std::endl;
	else
		throw(std::runtime_error("poll() failed"));
}

/*----------------------------------------------------------------------------*/

void	server::run()
{
	signal(SIGINT, ft_exit);
	signal(SIGQUIT, ft_exit);

	std::vector<struct pollfd>::iterator	it;

	for (;;)
	{
		it = _fds.begin();
		if (poll(&(*it), _fds.size(), -1) < 0)
			ft_exit(0);
		size_t nfd = _fds.size();
		for (size_t index = 0; index < nfd; index++)
		{
			if((_fds[index].revents & POLLIN) == POLLIN)
			{
				if (index == 0)
					accept_user();
				else
					receive_data(index);
			}
			else if (index != 0)
				ping(_users[index - 1], index);
		}
	}
}

/*----------------------------------------------------------------------------*/

void server::accept_user()
{
	struct pollfd		new_pollfd;
	sockaddr_in			address;
	socklen_t			len = sizeof(sockaddr);
	std::stringstream	nickname;
	std::stringstream	username;

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

	nickname << "Guest" << new_pollfd.fd;
	username << "Username" << new_pollfd.fd;
	std::cout << "New incoming connection - " << nickname.str()<< std::endl;
	std::string hostname = inet_ntoa(reinterpret_cast<sockaddr_in*>(&address)->sin_addr);
	user *new_user = new user(hostname, nickname.str(), username.str(), Socket(new_pollfd.fd, address, len), false);
	_users.push_back(new_user);
	_fds.push_back(new_pollfd);
}

/*----------------------------------------------------------------------------*/

void	server::close_user(size_t index)
{
	std::vector<user*>::iterator it = (_users.begin() + (index - 1));
	remove_user_from_channels(*it, "QUIT");
	std::cout << (*it)->getNickname() << " deconnexion" << std::endl; 
	delete *it;
	close(_fds[index].fd);
	_fds.erase(_fds.begin() + index);
	_users.erase(_users.begin() + (index - 1));
}

/*----------------------------------------------------------------------------*/

void	server::receive_data(size_t index)
{
	char	buf[512];
	std::string err;
	std::string tmp;
	int ret = 0;

	_users[index - 1]->setLastEvent(time(NULL));
	memset(&buf, 0, sizeof(buf));
	if((ret = recv(_fds[index].fd, buf, sizeof(buf), 0)) <= 0)
	{
		if (ret < 0)
		std::cerr << "recv() failed" << std::endl; 
		close_user(index);
		return ;
	}
	if (ret >= 512) 
	{
		err =  "Error : too long message.";
		send(_users[index - 1]->getSock(), err.c_str(), err.length(), 0);
		_users[index - 1]->buf.clear();
		return ;
	}

	tmp = buf;
	_users[index - 1]->buf += tmp;
	if (tmp.find("\n"))
	{
		std::cout << _users[index - 1]->getNickname() << " send : [" <<  _users[index - 1]->buf << "]" << std::endl;

		if (_users[index - 1]->buf.find("PASS") <= 4 && _users[index - 1]->getisLogged() == false)
		{
			_interpret.treat_user_buffer(*_users[index - 1]);
			_users[index - 1]->buf.clear();
			return ;
		}
		if (_users[index - 1]->getisLogged() == true)
			ret = _interpret.treat_user_buffer(*_users[index - 1]);
		if (_users[index - 1]->getisLogged() == false)
		{
			if (_users[index - 1]->getWasLogged() == false)
				send_replies(_users[index - 1], "You need to be log (PASS command).", "");
			close_user(index);
		}
	}
}

/*----------------------------------------------------------------------------*/

void	server::remove_user_from(user * usr, const std::string & name, const std::string & msg)
{
	std::string	str;

	if (channels[name]->getUsers().find(usr) != channels[name]->getUsers().end())
	{
		if (msg == "QUIT")
			str = " QUIT :disconnected";
		if (msg == "PART")
			str = " PART " + name;
		std::string quit = ":" + usr->getNickname() + str + "\r\n";
		for(std::set<user *>::iterator it = channels[name]->getUsers().begin(); it != channels[name]->getUsers().end(); ++it)
		{
			if (usr->getNickname() != (*it)->getNickname())
				send((*it)->getSock(), quit.c_str(), quit.length(), 0);
		}
		channels[name]->removeUser(*usr);
		usr->leave_channel(name);
		if (channels[name]->getUsers().empty())	// <-- remove channel if its users vector is empty
		{	
			delete channels[name];
			channels.erase(name);
		}
	}
}

/*----------------------------------------------------------------------------*/

void	server::remove_user_from_channels(user * usr, const std::string & msg)
{
	std::string name;
	while (!(usr->getChannels().empty()))
	{
		name = usr->getChannels().begin()->first;
		remove_user_from(usr, name, msg);
	}
}

/*----------------------------------------------------------------------------*/

void	server::send_replies(user *usr, const std::string & msg, const char* code) const
{
	std::string replies;
	std::string prefix = ":" + usr->getUsername();

	replies += (prefix +  " " + code + " " + usr->getNickname() + " " + msg + "\r\n");
	send(usr->getSock(), replies.c_str(), replies.length(), 0);
}

/*----------------------------------------------------------------------------*/

int	server::send_msg_to_user(const user * expeditor, const user * dest, const std::string & msg, const std::string & chan_name) const
{
	if (!dest)
		return 1;

	if (dest->isAway())
		send(expeditor->getSock(), &dest->getAfkString(), dest->getAfkString().size(), 0);

	std::string fmsg; 
	if (expeditor->getNickname() != dest->getNickname())
	{
		if (chan_name == "")
			fmsg = ":" + expeditor->getNickname() + " PRIVMSG " + dest->getNickname() + " :" + msg + "\r\n";
		else
			fmsg = ":" + expeditor->getNickname() + " PRIVMSG " + chan_name + " :" + msg + "\r\n";
	}

	send(dest->getSock(), fmsg.c_str(), fmsg.size(), 0);	// add error case
	return 0;
}

/*----------------------------------------------------------------------------*/

int	server::send_msg_to_channel(const user * expeditor, const Channel * dest, const std::string & msg) const
{
	if (!dest || !(expeditor->isMember(dest->getName())))
		return 1;

	std::set<user*>	userlist(dest->getUsers());
	int	ret = 0;

	for	(std::set<user*>::iterator it = userlist.begin(); ret == 0 && it != userlist.end(); it++)
	{
		ret = send_msg_to_user(expeditor, *it, msg, dest->getName());
	}
	return ret;
}

/*----------------------------------------------------------------------------*/

void	server::ping(user * usr, int index)
{
	double	timediff = difftime(time(NULL), usr->getLastEvent());

	if (timediff  > static_cast<double>(INACTIVE_SEC))
	{
		if (usr->getHasToPong())
		{
			return close_user(index);
		}
		else
		{
			std::string reply = (":" + _name + " PING " + usr->getUsername() + " \r\n");
			send(usr->getSock(), reply.c_str(), reply.length(), 0);
			usr->setHasToPong(time(NULL));
		}
	}
	return ;
}

void	server::pong(const std::string& username)
{
	for (std::vector<user*>::iterator it(_users.begin()); it != _users.end(); it++)
	{
		if ((*it)->getUsername() == username)
			return (*it)->pong();
	}
}

/*----------------------------------------------------------------------------*/

bool		server::isUser(const std::string & nickname) const
{ 
	for (std::vector<user*>::const_iterator it = _users.begin() ; it != _users.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return true; 
	}
	return false;
}

/*----------------------------------------------------------------------------*/

int 				server::getSock() const { return _socket.fd; }
std::string 		server::getName() const { return _name; }
std::string	 		server::getPort() const { return _strport; }
std::string 		server::getPassword() const { return _password; }
std::vector<user*>	server::getUsers() const { return _users; }

user *				server::getUser(const std::string & nickname) const
{
	for (std::vector<user*>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			return *it;
	}
	return NULL;
}

/*----------------------------------------------------------------------------*/

Channel *	server::getChannel(const std::string & name) const
{
	std::map<std::string, Channel* >::const_iterator	it(channels.find(name));

	if (it != channels.end())
		return it->second;
	return NULL;
}

/*----------------------------------------------------------------------------*/

std::string&	nameCaseIns(std::string& name) {
	for (int index = 0; name[index] ; ++index)
		name[index] = static_cast<char>(std::tolower(name[index]));
	return name;
}

/*----------------------------------------------------------------------------*/

void	server::welcomeNewUser(user * usr) 
{
	if (usr->getisLogged() == true)
	{
		std::string str = ":" + usr->getUsername() +  " " + RPL_WELCOME + " " + usr->getNickname() + \
			" Welcome to the Internet Relay Network " + usr->getUsername() + "\r\n";
		send(usr->getSock(), str.c_str(), str.length(), 0);
		
		str = ":" + usr->getUsername() +  " " + RPL_YOURHOST + " Your host is ircserv (" + this->getName() + \
			") running on port " + getPort() + "\r\n";
		send(usr->getSock(), str.c_str(), str.length(), 0);

		// str = ":" + usr->getUsername() +  " " + RPL_CREATED + " This server was created " + "" + "\r\n"; ////
		// send(usr->getSock(), str.c_str(), str.length(), 0);

		// str = ":" + usr->getUsername() +  " " + RPL_MYINFO + "ircserv has " "\r\n"; ////
		// send(usr->getSock(), str.c_str(), str.length(), 0);
	}
}
