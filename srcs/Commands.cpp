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
		return 0;
	}
	else if (pass->compare(serv->getPassword()) != 0)
	{
		serv->send_replies(_expeditor, ":Password incorrect", ERR_PASSWDMISMATCH); // pas dans la rfc au loggin maybe a remove
		return 0;
	}
	if (_expeditor->getisLogged() == false)
		_expeditor->setLogged(true);
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
	if (*arg == "anonymous") //// check char spe/grammar protocol rfc
		serv->send_replies(_expeditor, " :Erroneus nickname", ERR_ERRONEUSNICKNAME);
	if (serv->isUser(*arg) == false)
	{
		std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *arg + "\r\n";
	    send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
		_expeditor->setNickname(*arg);
		return 0;
	}
	else
		serv->send_replies(_expeditor, *arg + " :Nickname is already in use", ERR_NICKNAMEINUSE);
	return 0;
}
// ERR_NICKCOLLISION osef ?


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
	// 	return 0;
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
		return 0;
	}
	std::string	name(nameCaseIns(*channel));

	if (name.find('#') && name.find('&') == std::string::npos && name.find('+') == std::string::npos
	&& name.find('!') == std::string::npos)
	{
		serv->send_replies(_expeditor, "No such channel (need a chan mask)", ERR_BADCHANMASK);
		return 0;
	}
	if (_expeditor->getChannels().size() == 10)
	{
		serv->send_replies(_expeditor, name + " :You have joined too many channels (10 max)", ERR_TOOMANYCHANNELS);
		return 0;
	}
	if (!_expeditor->isMember(name) && serv->channels.find(name) == serv->channels.end())
	{
		std::cout << "Channel : " << name << " created" << std::endl;
		serv->channels[name] = new Channel(*_expeditor, name);
		serv->channels[name]->addUser(*_expeditor);
		_expeditor->join_channel(name, true);
		// _expeditor->setLocation(name);
		_expeditor->promote(name);
		//Voir si d'autre prefix possible que @
		std::string replies = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
		send(_expeditor->getSock(), replies.c_str(), replies.length(), 0);
		serv->getChannel(name)->send_names_replies(_expeditor);
		serv->send_replies(_expeditor, name + " :End of names list", RPL_ENDOFNAMES);
	}
	else if (_expeditor->getChannels().find(name) == _expeditor->getChannels().end())
	{
		std::string msg;
		std::set<user *>::iterator it;
		for(it = serv->channels[name]->getUsers().begin(); it != serv->channels[name]->getUsers().end(); ++it)
		{
			msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
			send((*it)->getSock(), msg.c_str(), msg.length(), 0);
		}
		msg = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
		send(_expeditor->getSock(), msg.c_str(), msg.length(), 0);
		serv->send_replies(_expeditor, + " " + name + " :" + serv->channels[name]->getTopic(), RPL_TOPIC);
		serv->getChannel(name)->send_names_replies(_expeditor);
		serv->send_replies(_expeditor, name + " :End of names list", RPL_ENDOFNAMES);
		_expeditor->join_channel(name, true);
		serv->channels[name]->addUser(*_expeditor);
		// _expeditor->setLocation(name);
	}
	return 0;
}
//ERR_BANNEDFROMCHAN
//ERR_INVITEONLYCHAN
//ERR_CHANNELISFULL
//ERR_NOSUCHCHANNEL
//ERR_BADCHANNELKEY


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
			// serv->send_replies(_expeditor, it->first + " <# visible> :" + it->second->getTopic() , RPL_LIST);
			serv->send_replies(_expeditor, it->first + ":" + it->second->getTopic() , RPL_LIST);
		}
	}
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
	{
		serv->send_replies(_expeditor, ":No origin specified", ERR_NOORIGIN);
		return 0;
	}
	if (arg->compare(serv->getName()) != 0)
		serv->send_replies(_expeditor, serv->getName() + ":No such server", ERR_NOSUCHSERVER);
	else
	{
		std::string reply = (":" + _expeditor->getUsername() + " PONG " + serv->getName() + " \r\n");
		send(_expeditor->getSock(), reply.c_str(), reply.length(), 0);
	}
	return 0;
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
		_expeditor->setLogged(false);
		return 0;
	}
	serv->remove_user_from_channels(_expeditor, " QUIT :" + *arg);
	_expeditor->setLogged(false);
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
	///// Iterrer dans les args avec nouveau parsing lorsque plusieurs noms de chans en une cmd
	std::string	*	arg = _args[CHANNEL].arg;
	std::cout << "PART ARG =" << *arg << std::endl;
	if (!arg)
	{
		serv->send_replies(_expeditor, "PART :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 0;
	}
	*arg = nameCaseIns(*arg);
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
	serv->remove_user_from(_expeditor, *arg, "PART");
	return 0;
}


/*	VERSION	*/

Version::Version():Command() {}
Version::~Version(){}
Version & Version::operator=(const Version & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Version::Version(server * serv):Command(serv) {}
int Version::execute()
{
	serv->send_replies(_expeditor, " : " + serv->getName() + " running version 1.42", RPL_VERSION);
	return 0;
}


/*	INFO	*/

Info::Info():Command() {}
Info::~Info(){}
Info & Info::operator=(const Info & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Info::Info(server * serv):Command(serv) {}
int Info::execute()
{
	serv->send_replies(_expeditor, " : " + serv->getName() + " running 1.42", RPL_INFO);
	serv->send_replies(_expeditor, " :End of /INFO list", RPL_ENDOFINFO);
	return 0;
}


/*	WHOIS */

Whois::Whois():Command() 
{
	_args[NICK].isNeeded = true;
}
Whois::~Whois(){}
Whois & Whois::operator=(const Whois & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Whois::Whois(server * serv):Command(serv) 
{
	_args[NICK].isNeeded = true;
}

int Whois::execute()
{
	std::string	*	arg = _args[NICK].arg;

// RPL_WHOISUSER                   RPL_WHOISCHANNELS
// RPL_WHOISCHANNELS               RPL_WHOISSERVER
// RPL_AWAY                        RPL_WHOISOPERATOR
// RPL_WHOISIDLE                   ERR_NOSUCHNICK
// RPL_ENDOFWHOIS

	if (!arg)
	{
		serv->send_replies(_expeditor, " :No nickname given", ERR_NONICKNAMEGIVEN);
		return 1;
	}
	if (serv->isUser(*arg) != false)
	{
		std::string str = ": " + _expeditor->getNickname() + " " + _expeditor->getUsername() + 
			+ " " + _expeditor->getHostname() + "\r\n";
	    send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
		return 0;
	}
	return 1;
}

// /*	WHO	*/

// Who::Who():Command() {}
// Who::~Who(){}
// Who & Who::operator=(const Who & x)
// {
// 	if (this != &x)
// 		serv = x.serv;
// 	return *this;
// }

// Who::Who(server * serv):Command(serv) {}
// int Who::execute()
// {
// 	return 0;
// }