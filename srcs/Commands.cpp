#include "../incs/Command.hpp"
#include "../incs/Commands.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"

#include <stdlib.h>

/*	PASS	*/

Pass::Pass():Command()
{
	_args[PASS].isNeeded = true;
}

Pass::~Pass() {}

Pass & Pass::operator=(const Pass & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Pass::Pass(server * serv):Command(serv)
{
	_args[PASS].isNeeded = true;
}

int Pass::execute()
{
	std::string	*	pass = _args[PASS].arg;
	// if (usr->isRegister == true) // voir si on garde un historic pas sur de capter voir 4.1.1
	// 	 serv->send_replies(usr, NULL, ERR_ALREADYREGISTRED);
	if (!pass)
	{
		serv->send_replies(_expeditor, "PASS :Not enough parameters", ERR_NEEDMOREPARAMS);
		return -1;
	}
	else if (pass->compare(serv->getPassword()) != 0)
	{
		serv->send_replies(_expeditor, ":Password incorrect", ERR_PASSWDMISMATCH); // pas dans la rfc au loggin maybe a remove
		return -1;
	}
	if (_expeditor->getisLogged() == false)
	{
		_expeditor->setPassword(*pass);
		_expeditor->setLogged(true);
	}
	return 0;
}

/*	NICK	*/

Nick::Nick():Command()
{
	_args[NICK].isNeeded = true;
}

Nick::~Nick() {}

Nick & Nick::operator=(const Nick & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Nick::Nick(server * serv):Command(serv)
{
	_args[NICK].isNeeded = true;
}

int Nick::execute()
{
	std::string	*	arg = _args[NICK].arg;

	if (!arg)
	serv->send_replies(_expeditor, " :No nickname given", ERR_NONICKNAMEGIVEN);
	if (serv->isUser(*arg) == false)
	{
		std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *arg + "\r\n";
	    send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
		_expeditor->setNickname(*arg);
		return 0;
	}
	else
		serv->send_replies(_expeditor, *arg + " :Nickname is already in use", ERR_NICKNAMEINUSE);

	// ERR_ERRONEUSNICKNAME if non conforme 'anonymous' ou char spe voir grammar protocol d'apres rfc
	// ERR_NICKCOLLISION osef ?
	return 1;
}

/*	USER	*/

User::User():Command()
{
	_args[NICK].isNeeded = true;
}

User::~User(){}

User & User::operator=(const User & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

User::User(server * serv):Command(serv)
{
	_args[NICK].isNeeded = true;
}

int User::execute()
{
	// Parsing issue
	std::string	*	arg = _args[NICK].arg;

	if (!arg) 
	{
		serv->send_replies(_expeditor, "USER :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 0;
	}
	// if (_expeditor->getisLogged() != true)
	// {

	// 	std::string prefix = ":" + _expeditor->getUsername();
	// 	std::string logged = prefix +  " " + RPL_WELCOME + " " + _expeditor->getUsername() + \
	// 		" Welcome to the Internet Relay Network :" + _expeditor->getUsername() + "\r\n";
	// 	send(_expeditor->getSock(), logged.c_str(), logged.length(), 0);	
	// }
	// if (_expeditor->getisLogged() == true)
	// 	serv->send_replies(_expeditor, " :You may not reregister", ERR_ALREADYREGISTRED);
	return 0;
}


/*	JOIN	*/

Join::Join():Command()
{
	_args[CHANNEL].isNeeded = true;
}

Join::~Join(){}

Join & Join::operator=(const Join & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Join::Join(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
}


int	Join::execute()
{
	std::string	*	channel = _args[CHANNEL].arg;

	if (!channel)
	{
		serv->send_replies(_expeditor, "JOIN :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 1;
	}
	std::string	name(*channel);

	if (name.find("#") && name.find("&") == std::string::npos && name.find("+") == std::string::npos 
	&& name.find("!!") == std::string::npos) // A MODIFIER (check juste premier char)
	{
		serv->send_replies(_expeditor, "No such channel (need a chan mask)", ERR_BADCHANMASK);
		return 1;
	}
	if (!_expeditor->isMember(name) && serv->channels.find(name) == serv->channels.end()
		&& _expeditor->getisLogged())
	{
		std::cout << "Channel : " << name << " created" << std::endl;
		// _expeditor->setOperator(true);
		serv->create_channel(*_expeditor, name);
		_expeditor->setLocation(name);

	}
	else if (_expeditor->getisLogged() && _expeditor->getLocation() != name)
	{
		std::string msg;
		std::string usersInChan;
		std::set<user *>::iterator it;
		for(it = serv->channels[name]->getUsers().begin(); it != serv->channels[name]->getUsers().end(); ++it)
		{
			msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
			usersInChan += (*it)->getNickname() + " ";
			send((*it)->getSock(), msg.c_str(), msg.length(), 0);
		}

		msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
		send(_expeditor->getSock(), msg.c_str(), msg.length(), 0);
		serv->send_replies(_expeditor, ":" + serv->channels[name]->getTopic(), RPL_TOPIC);
		serv->send_replies(_expeditor, "= " + name + " :" + usersInChan, RPL_NAMREPLY);
		serv->send_replies(_expeditor, name + " :End of names list", RPL_ENDOFNAMES);
		_expeditor->join_channel(name, true);
		serv->channels[name]->addUser(*_expeditor);
		_expeditor->setLocation(name);
	}
	return 0;
}
//ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
//ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//ERR_CHANNELISFULL               ERR_BADCHANMASK
//ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//RPL_TOPIC



/*	LIST	*/

List::List():Command()
{
	_args[CHANNEL].isNeeded = true;
}

List::~List(){}

List & List::operator=(const List & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

List::List(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
}

int List::execute()
{
	std::string	*	arg = _args[CHANNEL].arg;

	std::string list_msg;
	std::map<std::string, Channel*>::iterator it;
	serv->send_replies(_expeditor, "Channel :Users  Name", RPL_LISTSTART);
	if (!arg)
	{
		for (it = serv->channels.begin(); it != serv->channels.end(); ++it)
		{
			// serv->send_replies(_expeditor, "<channel> <# visible> :<topic>", RPL_LIST);
			serv->send_replies(_expeditor, it->first + " <# visible> :" + it->second->getTopic() , RPL_LIST);
		}
	}
	if (serv->channels.empty() == true)
		serv->send_replies(_expeditor, ":End of /LIST", RPL_LISTEND);
	if (arg && (serv->channels.find(*arg) != serv->channels.end()))
	{
		it = serv->channels.find(*arg);
		serv->send_replies(_expeditor, it->first + " <# visible> :" + it->second->getTopic() , RPL_LIST);
	}
	serv->send_replies(_expeditor, ":End of /LIST", RPL_LISTEND);

	return 0;
}

/*	PING	*/

Ping::Ping():Command()
{
	_args[HOSTNAME].isNeeded = true;
}

Ping::~Ping(){}

Ping & Ping::operator=(const Ping & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Ping::Ping(server * serv):Command(serv)
{
	_args[HOSTNAME].isNeeded = true;
}

int Ping::execute()
{
	std::string	*	arg = _args[HOSTNAME].arg;

	if (!arg)
		serv->send_replies(_expeditor, ":No origin specified", ERR_NOORIGIN);
	if (arg->compare(serv->getName()) != 0)
		serv->send_replies(_expeditor, serv->getName() + ":No such server", ERR_NOSUCHSERVER);
	else
	{
		std::string reply = (":" + _expeditor->getUsername() + " PONG " + serv->getName() + " \r\n");
		send(_expeditor->getSock(), reply.c_str(), reply.length(), 0);
		return 0;
	}
	return 1;
}


/*	QUIT	*/

Quit::Quit():Command()
{
	_args[MESSAGE].isNeeded = true;
}

Quit::~Quit(){}

Quit & Quit::operator=(const Quit & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Quit::Quit(server * serv):Command(serv)
{
	_args[MESSAGE].isNeeded = true;
}

int Quit::execute()
{
	std::string	*	arg = _args[MESSAGE].arg;

	if (!arg)
	{
		serv->user_leave_chan(_expeditor, _expeditor->getLocation(), true, "");
		// serv->close_user(_expeditor->getSock() - 1, *arg); // msg 
		return 0;
	}
	serv->user_leave_chan(_expeditor, _expeditor->getLocation(), true, *arg);
	return 0;
}


/*	PART	*/


Part::Part():Command()
{
	_args[CHANNEL].isNeeded = true;
}

Part::~Part(){}

Part & Part::operator=(const Part & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Part::Part(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
}

int Part::execute()
{
	std::string	*	arg = _args[CHANNEL].arg;

	if (!arg)
	{
		serv->send_replies(_expeditor, "PART :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 0;
	}
	if (_expeditor->getChannels().find(*arg) == _expeditor->getChannels().end())
	{
		serv->send_replies(_expeditor, *arg + " :You're not on that channel", ERR_NOTONCHANNEL);
		return 0;
	}
	if (serv->channels.find(*arg) == serv->channels.end())
	{
		serv->send_replies(_expeditor, *arg + " :No such channel", ERR_NOSUCHCHANNEL);
		return 0;
	}		
	serv->user_leave_chan(_expeditor, *arg, false, "");
	return 0;
}