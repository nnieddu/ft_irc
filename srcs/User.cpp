
#include "../incs/User.hpp"
#include "../incs/Socket.hpp"
#include "../incs/Channel.hpp"
#define RPL_WELCOME "001"

user::user(std::string hostname, std::string nickname, std::string username, const Socket & socket, bool server_operator)
: _hostname(hostname), _nickname(nickname), _username(username), _socket(socket), _server_operator(server_operator), _isLogged(false)
{}

user::~user() {}

std::string	user::getNickname() const { return _nickname; }
std::string	user::getUsername() const { return _username; }
std::string	user::getPassword() const { return _password; }
std::string	user::getHostname() const { return _hostname; }
// std::string	user::getPrefix() const { return _prefix; }
bool		user::getisLogged() const { return _isLogged; }
int 		user::getSock() const { return _socket.fd; }
std::string	user::getLocation() const { return _location; }
Socket		user::getSocket() const { return _socket; }
std::map<std::string, unsigned int>	user::getChannels() const { return _channels; }

void		user::setPassword(std::string psw) { _password = psw; }
void		user::setNickname(std::string nick) { _nickname = nick; }
void		user::setLocation(std::string & name) { _location = name; }
// void		user::setPrefix(std::string & prefix) { _prefix += prefix; }

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

bool	user::isOperator(const std::string & name) const
{
	if ((_channels.find(name)->second & o) == o)
		return true;
	else
		return false;
}

void	user::promote(const std::string & name)
{
	try
	{
		_channels.at(name) = _channels.at(name) | o;

	}
	catch(std::out_of_range &oor){} // <-- rentre ici si "name" n existe pas, voir si il y a un send a faire
}

void	user::demote(const std::string & name)
{
	try
	{
		_channels.at(name) = _channels.at(name) & ~o;
	}
	catch(std::out_of_range &oor){} // <-- rentre ici si "name" n existe pas, voir si il y a un send a faire
}

void	user::join_channel(const std::string & name, bool op)
{
	if (op)
		_channels.insert(std::make_pair(name, o));
	else
		_channels.insert(std::make_pair(name, 0));
}

void	user::leave_channel(const std::string & name)
{
	_channels.erase(name);
}

bool	user::isMember(const std::string & name) const
{
	return (_channels.find(name) != _channels.end());
}

bool user::isServOp() const {
	return _server_operator;
}

void user::promoteServ() {
	_server_operator = true;
}

void user::demoteServ() {
	_server_operator = false;
}

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
	catch(std::out_of_range &oor){} // <-- rentre ici si "name" n existe pas, voir si il y a un send a faire
}

void user::delVoice(const std::string &name) {
	try
	{
		_channels.at(name) = _channels.at(name) & ~v;
	}
	catch(std::out_of_range &oor){} // <-- rentre ici si "name" n existe pas, voir si il y a un send a faire
}

bool user::isRestrict() const {
	return _restricted;
}

void user::setRestrict() {
	_restricted = true;
}

void user::delRestrict() {
	_restricted = false;
}

bool user::isInvisible() const {
	return _invisible;
}

void user::setInvisible() {
	_invisible = true;
}

void user::delInvisible() {
	_invisible = false;
}

bool user::wallOp() const {
	return _wallOp;
}

void user::setWallOp() {
	_wallOp = true;
}

void user::delWallOp() {
	_wallOp = false;
}

const std::string &user::getAfkString() {
	return _afkMessage;
}
