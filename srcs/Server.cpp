
#include "../incs/Server.hpp"

server::server(const int & port, const std::string & password)
: _name("ft_irc.ircserv"), _port(port), _password(password), _interpret(this)
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

int 				server::getSock() const { return _socket.fd; }
std::string 		server::getName() const { return _name; }
std::string 		server::getPassword() const { return _password; }
std::vector<user*>	server::getUsers() const { return _users; }

user *				server::getUser(const std::string & nickname)
{
	for (std::vector<user*>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
			return *it;
	}
	return NULL;
}

Channel *	server::getChannel(std::string & name)
{
	std::map<std::string, Channel* >::iterator	it(channels.find(name));

	if (it != channels.end())
		return it->second;
	return NULL;
}

bool		server::isUser(std::string nickname) const
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
			}								// /!\ revent_error ?
		}
	}
}

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

void	server::receive_data(size_t index)
{
	char	buf[512];
	std::string err;
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
	if (ret >= 512) 
	{
		err =  "Error : too long message.";
		send(_users[index - 1]->getSock(), err.c_str(), err.length(), 0);
		_users[index - 1]->buf.clear();
		return ;
	}

	tmp = buf;
	_users[index - 1]->buf += tmp;
	if (tmp.find("\n") != std::string::npos)
	{
		std::cout << _users[index - 1]->getNickname() << " send : [" <<  _users[index - 1]->buf << "]" << std::endl;

		ret = _interpret.launch(*_users[index - 1]);
		_users[index - 1]->buf.clear();
		if (_users[index - 1]->getisLogged() == false)
			close_user(index);
	}
}

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

void	server::remove_user_from(user * usr, const std::string & name, std::string msg)
{
	if (channels[name]->getUsers().find(usr) != channels[name]->getUsers().end())
	{
		if (msg == "QUIT")
			msg = " QUIT :disconnected";
		if (msg == "PART")
			msg = " PART " + name;
		std::string quit = ":" + usr->getNickname() + msg + "\r\n";
		for(std::set<user *>::iterator it = channels[name]->getUsers().begin(); it != channels[name]->getUsers().end(); ++it)
		{
			if (usr->getNickname() != (*it)->getNickname())
				send((*it)->getSock(), quit.c_str(), quit.length(), 0);
		}
		channels[name]->removeUser(*usr);
		if (channels[name]->getUsers().empty())	// <-- remove channel if its users vector is empty
		{	
			delete channels[name];
			channels.erase(name);
		}
	}
}

void	server::remove_user_from_channels(user * usr, std::string msg)
{
	while (!(usr->getChannels().empty()))
	{
		std::string name = usr->getChannels().begin()->first;
		remove_user_from(usr, name, msg);
		usr->leave_channel(name);
	}
}

void	server::send_replies(user *usr, std::string msg, const char* code)
{
	std::string replies;
	std::string prefix = ":" + usr->getUsername(); // usr->getUsername();
	// msg += prefix;

	replies += (prefix +  " " + code + " " + usr->getNickname() + " " + msg + "\r\n");
	send(usr->getSock(), replies.c_str(), replies.length(), 0);
}

int	server::send_msg_to_user(user * expeditor, user * dest, std::string & msg, std::string chan_name)
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

int	server::send_msg_to_channel(user * expeditor, Channel * dest, std::string & msg)
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

std::string&	nameCaseIns(std::string& name) {
	for (int index = 0; name[index] ; ++index)
		name[index] = static_cast<char>(std::tolower(name[index]));
	return name;
}
