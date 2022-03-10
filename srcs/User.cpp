
#include "../incs/User.hpp"
#include "../incs/Socket.hpp"
#include "../incs/Channel.hpp"
#include "../incs/Server.hpp"
#include "../incs/NumericReplies.hpp"

/*----------------------------------------------------------------------------*/

user::user(const std::string & hostname, const std::string & nickname, const std::string & username, const Socket & socket, bool server_operator)
:
	_hostname(hostname),
	_nickname(nickname),
	_username(username),
	_realname(username + "le reel"),
	_socket(socket),
	_server_operator(server_operator),
	_afk(false),
	_isLogged(false),
	_wasLogged(false),
	_last_event(time(NULL)),
	_has_to_pong(false),
	_logLvl(0),
	_channels(std::map<std::string, unsigned int>())
{}

user::~user() {}

/*----------------------------------------------------------------------------*/

void		user::setNickname(const std::string & nick) { _nickname = nick; }

void		user::setUsername(const std::string & username) { _username = username; }

void		user::setRealname(const std::string & realname) { _realname = realname; }

void		user::setLogged(bool log) { _isLogged = log; _wasLogged = true; }

void		user::setLogLvl(int loglvl) { _logLvl = loglvl; }

/*----------------------------------------------------------------------------*/

bool user::isServOp() const { return _server_operator; }

void user::promoteServ() {
	_server_operator = true;
}

void user::demoteServ() {
	_server_operator = false;
}

/*----------------------------------------------------------------------------*/

void	user::join_channel(const std::string & name, bool op)
{
	std::string	tmp(name);

	if (op)
		_channels.insert(std::make_pair(nameCaseIns(tmp), o));
	else
		_channels.insert(std::make_pair(nameCaseIns(tmp), 0));
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

void	user::setLastEvent(const std::time_t & now)
{
	_last_event = now;
}

void	user::setHasToPong(const std::time_t & now)
{
	_last_event = now;
	_has_to_pong = true;
}

std::time_t	user::getLastEvent() const
{
	return _last_event;
}

bool	user::getHasToPong() const
{
	return _has_to_pong;
}

void	user::pong()
{
	_has_to_pong = false;
}

/*----------------------------------------------------------------------------*/

void	user::send_who_reply(const user * receiver, const Channel * chan) const
{
	bool		anonymous = false;
	std::string	name = "*";
	std::string info;

	if (chan)
	{
		anonymous = chan->geta();
		name = chan->getName();
	}

	info = name + " " + getUsername(anonymous) + " " + getHostname(anonymous) + " 127.0.0.1 " + getNickname(anonymous) \
		+ "H :0 " + getRealname(anonymous);

	std::string replies = ":" + receiver->getUsername() +  " " + RPL_WHOREPLY + " " \
		+ receiver->getNickname() + " WHO :" + info + "\r\n";

	send(receiver->getSock(), replies.c_str(), replies.length(), 0);
}

/*----------------------------------------------------------------------------*/

std::string							user::getNickname(bool isAnonymous) const
{
	if (isAnonymous)
		return "anonymous";
	return _nickname;
}

std::string							user::getUsername(bool isAnonymous) const
{
	if (isAnonymous)
		return "anonymous";
	return _username;
}

std::string							user::getHostname(bool isAnonymous) const
{
	if (isAnonymous)
		return "anonymous";
	return _hostname;
}

std::string							user::getRealname(bool isAnonymous) const
{
	if (isAnonymous)
		return "anonymous";
	return _realname;
}

int									user::getLogLvl() const { return _logLvl; }
bool								user::getisLogged() const { return _isLogged; }
bool								user::getWasLogged() const { return _wasLogged; }
int 								user::getSock() const { return _socket.fd; }
Socket								user::getSocket() const { return _socket; }
std::map<std::string, unsigned int>	user::getChannels() const { return _channels; }
