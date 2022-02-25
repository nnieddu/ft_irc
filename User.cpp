
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

void	user::clearbuf()
{
	buf.clear();
}

std::ostream& operator<<(std::ostream& os, const user& user)
{
    os << user.getNickname() << " | " << user.getUsername() << " | " << user.getSock() << std::endl;
    return os;
}