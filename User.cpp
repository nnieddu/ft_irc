
#include "User.hpp"
#include "Socket.hpp"

user::user(std::string name, std::string password, bool isOperator, int serverSocket)
: _name(name), _nickname(name), _password(password), _isOperator(isOperator) 
{
	_socket.len = 0;
	_socket.fd = accept(serverSocket, reinterpret_cast<sockaddr*>(&_socket.address), &_socket.len);
	if (_socket.fd == -1)
		throw(std::runtime_error("accept"));
}

user::~user() {}

std::string	user::getName() const
{
	return _name;
}

std::string	user::getNickname() const
{
	return _nickname;
}

int user::getSock() const
{
	return _socket.fd;
}