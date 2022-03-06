#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Pass::~Pass() {}

Pass & Pass::operator=(const Pass & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Pass::Pass(server * serv):Command(serv)
{
	_args[PASS].isNeeded = true;
}

/*----------------------------------------------------------------------------*/

void Pass::execute()
{
	std::string	*	pass = _args[PASS].arg;
	// if (usr->isRegister == true) // voir si on garde un historic pas sur de capter voir 4.1.1
	// 	 _serv->send_replies(usr, NULL, ERR_ALREADYREGISTRED);
	if (!pass)
	{
		_serv->send_replies(_expeditor, "PASS :Not enough parameters", ERR_NEEDMOREPARAMS);
		return ;
	}
	else if (pass->compare(_serv->getPassword()) != 0)
	{
		_serv->send_replies(_expeditor, ":Password incorrect", ERR_PASSWDMISMATCH); // pas dans la rfc au loggin maybe a remove
		return ;
	}
	if (_expeditor->getisLogged() == false) //&& !pass->find("USER") && !pass->find("NICK"))
		_expeditor->setLogged(true);
	return ;
}
