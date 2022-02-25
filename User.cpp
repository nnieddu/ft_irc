
#include "User.hpp"
#include "Socket.hpp"

user::user(std::string nickname, std::string username, std::string password, bool isOperator, const Socket & socket)
: _nickname(nickname), _username(username), _password(password), _isOperator(isOperator), _socket(socket)
{}

user::user(const user& x)
:_nickname(x.getNickname()), _username(x.getUsername()), _password(x.getPassword()), _isOperator(x.IsOperator()), _socket(x.getSocket()), buf(x.buf)
{}

user& user::operator=(const user& x)
{
	if (this != &x)
	{
		_nickname = x.getNickname();
		_username = x.getUsername();
		_password = x.getPassword();
		_isOperator = x.IsOperator();
		_socket = x.getSocket();
		buf = x.buf;
	}
	return *this;
}

user::~user() {}

std::string	user::getNickname() const
{
	return _nickname;
}

std::string	user::getUsername() const
{
	return _username;
}

std::string	user::getPassword() const
{
	return _password;
}

bool	user::IsOperator() const
{
	return _isOperator;
}

Socket	user::getSocket() const
{
	return _socket;
}

int user::getSock() const
{
	return _socket.fd;
}


void	user::join_channel(std::string & name)
{
	_channels.insert(name);
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

void	user::clearbuf()
{
	buf.clear();
}

std::ostream& operator<<(std::ostream& os, const user& user)
{
    os << user.getNickname() << " | " << user.getUsername() << " | " << user.getSock() << std::endl;
    return os;
}