
#include "../incs/User.hpp"
#include "../incs/Socket.hpp"

user::user(std::string ip, std::string nickname, std::string username, std::string password, const Socket & socket, bool server_operator)
: _ip(ip), _nickname(nickname), _username(username), _password(password), _socket(socket), _server_operator(server_operator)
{}

user::~user() {}

std::string	user::getNickname() const { return _nickname; }
std::string	user::getUsername() const { return _username; }
std::string	user::getPassword() const { return _password; }

void		user::setPassword(std::string psw) { _password = psw; }
void		user::setNickname(std::string nick) { _nickname = nick; }

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

Socket	user::getSocket() const { return _socket; }

int user::getSock() const { return _socket.fd; }


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

std::string	user::getLocation() const		// /!\ locations related stuff
{
	return _location;
}

bool	user::isMember(std::string & name) const
{
	return (_channels.find(name) != _channels.end());
}
