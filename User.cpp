
#include "User.hpp"
#include "Socket.hpp"

user::user(std::string ip, std::string nickname, std::string username, std::string password, bool isOperator, const Socket & socket)
: _ip(ip), _nickname(nickname), _username(username), _password(password), _isOperator(isOperator), _socket(socket)
{}

user::~user() {}

std::string	user::getNickname() const { return _nickname; }

std::string	user::getUsername() const { return _username; }

std::string	user::getPassword() const { return _password; }

bool	user::IsOperator() const { return _isOperator; }

Socket	user::getSocket() const { return _socket; }

int user::getSock() const { return _socket.fd; }

void user::setOperator(bool opt) {  _isOperator = opt; }


void	user::join_channel(std::string & name)
{
	_channels.insert(name);
	setLocation(name);		// /!\ locations related stuff
}

void	user::leave_channel(std::string & name)
{
	_channels.erase(name);
	_isOperator = false;
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
