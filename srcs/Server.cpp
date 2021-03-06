#include "../incs/Server.hpp"

/*----------------------------------------------------------------------------*/

server::server(const int & port, const std::string & password)
: _name("ircserv"), _port(port), _password(password), _interpret(this), _index(0)
{
	if (_port <= 1023 || _port > 65535)
		throw(std::invalid_argument(std::string("port number")));
	if (_password.size() < 5 || _password.size() > 16)
		throw(std::invalid_argument(std::string("password")));

	_hostname = _socket.server_socket(_port);

	struct pollfd	serv_fd;
	memset(&serv_fd, 0, sizeof(serv_fd));
	serv_fd.fd = _socket.fd;
	serv_fd.events = POLLIN;

	_fds.push_back(serv_fd);
	srand(time(NULL));

	time_t now = time(0);
	std::string time = ctime(&now);
	tm *gmtm = gmtime(&now);
	gmtm->tm_hour++;
	_creation_date = asctime(gmtm);	
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

	for(;;)
	{
		if (!channels.empty())
			check_channels(time(NULL));
		it = _fds.begin();
		if (poll(&(*it), _fds.size(), 1) < 0)
			ft_exit(0);
		size_t nfd = _fds.size();
		for (_index = 0; _index < nfd; _index++)
		{
			if((_fds[_index].revents & POLLIN) == POLLIN)
			{
				if (_index == 0)
					accept_user();
				else
					receive_data(_index);
				if (_index == -1)
					break ;
				if (_index == -2)
					return ;
			}
			else if (_index != 0 && ping(_users[_index - 1], _index))
				break;
		}
	}
}

/*----------------------------------------------------------------------------*/
void	server::first_auth(user * usr)
{
	if ((usr->buf.find("PASS") <= 4 && usr->getLogLvl() == 0) || \
		(usr->buf.find("CAP") <= 3 && usr->getLogLvl() == 1) ||  \
			(usr->buf.find("NICK") <= 4 && usr->getLogLvl() == 1) || \
				(usr->buf.find("USER") <= 4 && usr->getLogLvl() == 2) || \
					usr->buf.find("QUIT") <= 4)
		_interpret.treat_user_buffer(*usr);
	if (!usr->buf.empty())
	{
		std::string replies = usr->getNickname() + \
		" :You need to be fully authentified before using other commands.\r\n";
		send(usr->getSock(), replies.c_str(), replies.length(), 0);
		usr->buf.clear();
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
	std::string hostname = inet_ntoa(reinterpret_cast<sockaddr_in*>(&address)->sin_addr);
	user *new_user = new user(hostname, nickname.str(), username.str(), Socket(new_pollfd.fd, address, len), false);
	_users.push_back(new_user);
	_fds.push_back(new_pollfd);
}

/*----------------------------------------------------------------------------*/

void	server::close_user(size_t index, const std::string & msg)
{
	std::vector<user*>::iterator it = (_users.begin() + (index - 1));
	remove_user_from_channels(*it, msg);
	delete *it;
	close(_fds[index].fd);
	_fds.erase(_fds.begin() + index);
	_users.erase(_users.begin() + (index - 1));
}

int	server::kill(user * expeditor, user * target, const std::string & msg)
{
	std::string	kill;
	size_t		index = 1;

	while (index < _fds.size() && _fds[index].fd != target->getSock())
		index++;

	if (expeditor)
		kill = ":" + expeditor->getNickname() + " KILL " + target->getNickname() + " :" + msg + "\r\n";
	else
		kill = ":" + _name + " KILL " + target->getNickname() + " :" + msg + "\r\n";

	send(target->getSock(), kill.c_str(), kill.length(), 0);
	
	if (expeditor)
		kill = "killed by " + expeditor->getNickname() + "(" + msg + ")";
	
	remove_user_from_channels(target, kill);
	delete target;
	close(_fds[index].fd);
	_fds.erase(_fds.begin() + index);
	_users.erase(_users.begin() + (index - 1));
	_index = -1;
	return 1;
}

/*----------------------------------------------------------------------------*/

void	server::receive_data(size_t index)
{
	char	buf[512];
	std::string err;
	std::string tmp;
	int ret;

	memset(&buf, 0, sizeof(buf));
	if((ret = recv(_fds[index].fd, buf, sizeof(buf), 0)) <= 0)
	{
		kill(NULL, getUser(_users[index - 1]->getNickname()), "Communication error");
		return ;
	}
	if (ret >= 512) 
	{
		send_replies(_users[index - 1] ,_users[index - 1]->getNickname() + ":Input line was too long" , ERR_INPUTTOOLONG);
		_users[index - 1]->buf.clear();
		return ;
	}

	tmp = buf;
	_users[index - 1]->buf += tmp;
	if (tmp.find('\n') != std::string::npos)
	{
		if (_users[index - 1]->getisLogged())
			_interpret.treat_user_buffer(*_users[index - 1]);
		else
			first_auth(_users[index - 1]);
	}
	if (_index != -1)
	{
		_users[index - 1]->setLastEvent(time(NULL));
		if (_users[index - 1]->getLogLvl() == -1)
			close_user(index, "QUIT");
	}
}

/*----------------------------------------------------------------------------*/

void	server::remove_user_from(user * usr, const std::string & name, const std::string & msg)
{
	std::string	str;
	std::string quit;
	
	if (channels[name]->getUsers().find(usr) != channels[name]->getUsers().end())
	{
		if (msg == "PART" || msg == "KICK")
			str = " " + msg + " " + name;
		else if (msg.find("killed by") != std::string::npos)
			str = " QUIT :" + msg;
		else if (msg.find("PART") != std::string::npos && msg.find(name) != std::string::npos)
			str = msg;
		else
			str = " QUIT :disconnected";
		if (channels[name]->geta())
		{
			str = " PART " + name;
			quit = ":" + usr->getNickname(true) + str + "\r\n";
		}
		else
			quit = ":" + usr->getNickname() + str + "\r\n";
		for(std::set<user *>::iterator it = channels[name]->getUsers().begin(); it != channels[name]->getUsers().end(); ++it)
		{
			if (usr != *it)
				send((*it)->getSock(), quit.c_str(), quit.length(), 0);
		}
		channels[name]->removeUser(*usr);
		usr->leave_channel(name);
		if (channels[name]->getUsers().empty())
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

void	server::send_replies(const user *usr, const std::string & msg, const char* code) const
{
	std::string replies;

	replies = ":" + usr->getUsername() +  " " + code + " " + usr->getNickname() + " " + msg + "\r\n";

	if (send(usr->getSock(), replies.c_str(), replies.length(), 0) == -1)
		std::cerr << strerror(errno) << std::endl;
}

/*----------------------------------------------------------------------------*/

int	server::send_msg_to_user(const user * expeditor, const user * dest, const std::string & msg, const std::string & chan_name, bool rpl, bool isAnonymous) const
{
	if (!dest)
	{
		if (rpl)
			send_replies(expeditor, "PRIVMSG :No such nick", ERR_NOSUCHNICK);
		return 0;
	}

	if (dest->isAway())
	{
		if (rpl)
			send_replies(expeditor, std::string( dest->getNickname() + " :" + dest->getAfkString()), RPL_AWAY);
		return 0;
	}

	std::string fmsg;

	if (chan_name.empty())
		fmsg = ":" + expeditor->getNickname() + " PRIVMSG " + dest->getNickname() + " :" + msg + "\r\n";
	else
		fmsg = ":" + expeditor->getNickname(isAnonymous) + " PRIVMSG " + chan_name + " :" + msg + "\r\n";
	return send(dest->getSock(), fmsg.c_str(), fmsg.size(), 0);
}

/*----------------------------------------------------------------------------*/

int	server::send_msg_to_channel(const user * expeditor, const Channel * dest, const std::string & msg) const
{
	std::set<user*>	userlist(dest->getUsers());

	if (!dest)
	{
		send_replies(expeditor, "PRIVMSG :cannot send to channel (channel not found)", ERR_CANNOTSENDTOCHAN);
		return 1;
	}
	if (dest->getn() && !(expeditor->isMember(dest->getName())))
	{
		send_replies(expeditor, "PRIVMSG :cannot send to channel (mode:+n)", ERR_CANNOTSENDTOCHAN);
		return 1;
	}
	if (dest->getm() && !expeditor->isOperator(dest->getName()) && !expeditor->isVoice(dest->getName()))
	{
		send_replies(expeditor, "PRIVMSG :cannot send to channel (mode:+m)", ERR_CANNOTSENDTOCHAN);
		return 1;
	}
	if (dest->getm() && expeditor->isRestrict())
	{
		send_replies(expeditor, "PRIVMSG :cannot send to channel (mode:+m + restricted user)", ERR_CANNOTSENDTOCHAN);
		return 1;
	}

	for	(std::set<user*>::iterator it = userlist.begin(); it != userlist.end(); it++)
	{
		if (expeditor == *it)
			continue;
		else if (send_msg_to_user(expeditor, *it, msg, dest->getName(), true, dest->geta()) == -1)
			return -1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/

int	server::ping(user * usr, int index)
{
	double	timediff = difftime(time(NULL), usr->getLastEvent());

	if (timediff  > static_cast<double>(INACTIVE_SEC))
	{
		if (usr->getHasToPong())
			return kill(NULL, getUser(_users[index - 1]->getNickname()), "Took too long to pong");
		else
		{
			std::string reply = (":" + _name + " PING " + usr->getUsername() + " \r\n");
			send(usr->getSock(), reply.c_str(), reply.length(), 0);
			usr->setHasToPong(time(NULL));
		}
	}
	return 0;
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

void	server::check_channels(const std::time_t & now)
{
	for (std::map<std::string, Channel* >::iterator	it(channels.begin()); it != channels.end(); it++)
	{
		if (it->second->getr() && it->second->mustAddOp(now))
			it->second->selectRandomOp(_name);
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
std::string 		server::getHostname() const { return _hostname; }
int			 		server::getPort() const { return _port; }
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

Interpret&	server::getInterpret() { return _interpret; }

/*----------------------------------------------------------------------------*/

std::string&	nameCaseIns(std::string& name) {
	for (int index = 0; name[index] ; ++index)
		name[index] = static_cast<char>(std::tolower(name[index]));
	return name;
}

/*----------------------------------------------------------------------------*/

std::string ft_itoa(int nbr)
{
	std::string str;
	size_t index;

	if (nbr == 0)
		return (std::string("0"));
	index = nbr;
	while (index)
	{
		str.insert(str.begin(), (index % 10) + 48);
		index /= 10;
	}
	return (str);
}

/*----------------------------------------------------------------------------*/

void	server::welcomeNewUser(user * usr) 
{
	if (usr->getisLogged())
	{
		time_t now = time(0);
		std::string time = ctime(&now);
		tm *gmtm = gmtime(&now);
		gmtm->tm_hour++;
		_creation_date = asctime(gmtm);	

		std::string str = ":" + usr->getUsername() + "@" + usr->getHostname() +  " " + RPL_WELCOME + " " + usr->getNickname() + \
			" Welcome to the Internet Relay Network " + usr->getNickname() + "\r\n";
		send(usr->getSock(), str.c_str(), str.length(), 0);
		
		str = ":" + usr->getUsername() + "@" + usr->getHostname() +  " " + RPL_YOURHOST + " Your host is " + this->getName() + \
			" running on port " + ft_itoa(getPort()) + "\r\n";
		send(usr->getSock(), str.c_str(), str.length(), 0);

		str = ":" + usr->getUsername() + "@" + usr->getHostname() +  " " + RPL_CREATED + " This server was created " + \
			_creation_date.erase(_creation_date.size() - 1, _creation_date.size()) + "\r\n";
		send(usr->getSock(), str.c_str(), str.length(), 0);

		str = ":" + usr->getUsername() + "@" + usr->getHostname() + " " + RPL_MYINFO + " [aiowr] User MODE and [aiklmnoprtv] Channel MODE are available\r\n";
		send(usr->getSock(), str.c_str(), str.length(), 0);

	}
}
