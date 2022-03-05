
#include "../incs/User.hpp"
#include "../incs/Socket.hpp"
#include "../incs/Channel.hpp"
#include "../incs/Server.hpp"
#include "../incs/NumericReplies.hpp"

/*----------------------------------------------------------------------------*/

user::user(const std::string & hostname, const std::string & nickname, const std::string & username, const Socket & socket, bool server_operator)
: _hostname(hostname), _nickname(nickname), _username(username), _socket(socket), _server_operator(server_operator), _isLogged(false)
{}

user::~user() {}

/*----------------------------------------------------------------------------*/

void		user::setNickname(std::string nick) { _nickname = nick; }

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

/*----------------------------------------------------------------------------*/

bool user::isServOp() const {
	return _server_operator;
}

void user::promoteServ() {
	_server_operator = true;
}

void user::demoteServ() {
	_server_operator = false;
}

/*----------------------------------------------------------------------------*/

void	user::join_channel(std::string & name, bool op)
{
	if (op)
		_channels.insert(std::make_pair(nameCaseIns(name), o));
	else
		_channels.insert(std::make_pair(nameCaseIns(name), 0));
}

void	user::leave_channel(const std::string & name)
{
	_channels.erase(name);
}

void	user::promote(const std::string & name)
{
	try
	{
		_channels.at(name) = _channels.at(name) | o;

	}
	catch(std::out_of_range &oor){}
}

void	user::demote(const std::string & name)
{
	try
	{
		_channels.at(name) = _channels.at(name) & ~o;
	}
	catch(std::out_of_range &oor){}
}

bool	user::isOperator(const std::string & name) const
{
	if ((_channels.find(name)->second & o) == o)
		return true;
	else
		return false;
}

bool	user::isMember(const std::string & name) const
{
	return (_channels.find(name) != _channels.end());
}

/*----------------------------------------------------------------------------*/

bool user::isVoice(const std::string &name) const {
	if ((_channels.find(name)->second & v) == v)
		return true;
	else
		return false;
}

void user::setVoice(const std::string &name) {
	try
	{
		_channels.at(name) = _channels.at(name) | v;
	}
	catch(std::out_of_range &oor){}
}

void user::delVoice(const std::string &name) {
	try
	{
		_channels.at(name) = _channels.at(name) & ~v;
	}
	catch(std::out_of_range &oor){}
}

/*----------------------------------------------------------------------------*/

bool user::isRestrict() const {
	return _restricted;
}

void user::setRestrict() {
	_restricted = true;
}

void user::delRestrict() {
	_restricted = false;
}

/*----------------------------------------------------------------------------*/

bool user::isInvisible() const {
	return _invisible;
}

void user::setInvisible() {
	_invisible = true;
}

void user::delInvisible() {
	_invisible = false;
}

/*----------------------------------------------------------------------------*/

bool user::wallOp() const {
	return _wallOp;
}

void user::setWallOp() {
	_wallOp = true;
}

void user::delWallOp() {
	_wallOp = false;
}

/*----------------------------------------------------------------------------*/

bool user::isAway() const {
	return _afk;
}

void user::setAfk(const std::string &afkStr) {
	_afk = true;
	_afkMessage = afkStr;
}

void user::delAfk() {
	_afk = false;
}

const std::string &user::getAfkString() const{
	return _afkMessage;
}

/*----------------------------------------------------------------------------*/

std::string							user::getNickname() const { return _nickname; }
std::string							user::getUsername() const { return _username; }
std::string							user::getHostname() const { return _hostname; }
bool								user::getisLogged() const { return _isLogged; }
int 								user::getSock() const { return _socket.fd; }
Socket								user::getSocket() const { return _socket; }
std::map<std::string, unsigned int>	user::getChannels() const { return _channels; }
