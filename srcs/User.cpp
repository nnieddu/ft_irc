
#include "../incs/User.hpp"
#include "../incs/Socket.hpp"
#define RPL_WELCOME "001"

user::user(std::string hostname, std::string nickname, std::string username, const Socket & socket, bool server_operator)
: _hostname(hostname), _nickname(nickname), _username(username), _socket(socket), _server_operator(server_operator), _isLogged(false)
{}

user::~user() {}

std::string	user::getNickname() const { return _nickname; }
std::string	user::getUsername() const { return _username; }
std::string	user::getPassword() const { return _password; }
bool		user::getisLogged() const { return _isLogged; }
bool		user::getLocation(std::string chan) const 
{
	if (chan == _location)
		return true;
	return false;
}
int 		user::getSock() const { return _socket.fd; }
Socket		user::getSocket() const { return _socket; }

void		user::setPassword(std::string psw) { _password = psw; }
void		user::setNickname(std::string nick) { _nickname = nick; }

// void		user::setOperator(bool op) { }
void		user::setLogged(bool log) 
{ 
	_isLogged = log; 
	if (log == true) 
	{
		std::string prefix = ":" + getUsername();
		std::string logged = prefix +  " " + RPL_WELCOME + " " + getNickname() + \
			" Welcome to the Internet Relay Network :" + getUsername() + "\r\n";
		send(getSock(), logged.c_str(), logged.length(), 0);		
	}
}

std::map<std::string, bool>	user::getChannels() const { return _channels; }

bool	user::isOperator(std::string & name) const
{
	return _channels.find(name)->second;
}

void	user::promote(std::string & name)
{
	try
	{
		_channels.at(name) = true;
	}
	catch(std::out_of_range &oor){}			// <-- rentre ici si "name" n existe pas, voir si il y a un send a faire
}

void	user::demote(std::string & name)
{
	try
	{
		_channels.at(name) = false;
	}
	catch(std::out_of_range &oor){}			// <-- rentre ici si "name" n existe pas, voir si il y a un send a faire
}

void	user::join_channel(std::string & name, bool op)
{
	_channels.insert(std::make_pair(name, op));
	setLocation(name);		// /!\ locations related stuff
}

void	user::leave_channel(std::string & name)
{
	_channels.erase(name);
}

void	user::setLocation(std::string & name)
{
	_location = name;
}

bool	user::isMember(std::string & name) const
{
	return (_channels.find(name) != _channels.end());
}
