#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Time::~Time(){}

Time & Time::operator=(const Time & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Time::Time(server * serv):Command(serv)
{
	_args[HOSTNAME].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Time::execute()
{
	std::string	*	arg = _args[HOSTNAME].arg;

	if (!arg || *arg == _serv->getName() || *arg == "ircserv")
	{
		time_t now = time(0);
		std::string time = ctime(&now);
		tm *gmtm = gmtime(&now);
		gmtm->tm_hour++;
		time = asctime(gmtm);
		_serv->send_replies(_expeditor, "ircserv(" + _serv->getName() + "): " + time, RPL_TIME);
		return ;
	}
	_serv->send_replies(_expeditor, *arg + " No such server", ERR_NOSUCHSERVER);
}