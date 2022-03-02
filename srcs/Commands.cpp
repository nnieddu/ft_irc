#include "../incs/Command.hpp"
#include "../incs/Commands.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"

#include <stdlib.h>

/*	PASS	*/

Pass::Pass():Command()
{
	_argument = true;
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
	_argument = true;
}

int Pass::execute()
{
	// if (usr->isRegister == true) // voir si on garde un historic pas sur de capter voir 4.1.1
	// 	 serv->send_replies(usr, NULL, ERR_ALREADYREGISTRED);
	if (!_arg)
	{
		serv->send_replies(_expeditor, "PASS :Not enough parameters", ERR_NEEDMOREPARAMS);
		return -1;
	}
	else if (_arg->compare(serv->getPassword()) != 0)
	{
		serv->send_replies(_expeditor, ":Password incorrect", ERR_PASSWDMISMATCH); // pas dans la rfc au loggin maybe a remove
		return -1;
	}
	if (_expeditor->getisLogged() == false)
	{
		_expeditor->setPassword(*_arg);
		_expeditor->setLogged(true);
	}
	return 0;
}

/*	NICK	*/

Nick::Nick():Command()
{
	_argument = true;
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
	_argument = true;
}

int Nick::execute()
{
	if (!_arg)
	serv->send_replies(_expeditor, ":No nickname given", ERR_NONICKNAMEGIVEN);
	if (serv->isUser(*_arg) == false)
	{
		std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *_arg + "\r\n";
	    send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
		_expeditor->setNickname(*_arg);
		return 0;
	}
	else
		serv->send_replies(_expeditor, *_arg + " :Nickname is already in use", ERR_NICKNAMEINUSE);

	// ERR_ERRONEUSNICKNAME if non conforme 'anonymous' ou char spe voir grammar protocol d'apres rfc
	// ERR_NICKCOLLISION osef ?
	return 1;
}

/*	USER	*/
/*
User::User():Command()
{
	_argument = true;
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
	_argument = true;
}

int User::execute()
{
	// Parsing issue 
	if (!_arg) 
	{

		serv->send_replies(_expeditor, "USER :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 0;
	}
	// if (_expeditor->getisLogged() == true)
	// 	serv->send_replies(_expeditor, ":You may not reregister", ERR_ALREADYREGISTRED);
	// if (_expeditor->getisLogged() == true) // le parsing bloque
	// {

	// 	std::string prefix = ":" + _expeditor->getUsername();
	// 	std::string logged = prefix +  " " + RPL_WELCOME + " " + _expeditor->getUsername() + \
	// 		" Welcome to the Internet Relay Network :" + _expeditor->getUsername() + "\r\n";
	// 	send(_expeditor->getSock(), logged.c_str(), logged.length(), 0);		
	// }
	return 0;
}
*/
/*	JOIN	*/

Join::Join():Command()
{
	// _argument = true;
	_chan = true;
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
	// _argument = true;
	_chan = true;
}


int	Join::execute()
{
	if (!_channel)
	{
		serv->send_replies(_expeditor, "JOIN :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 1;
	}
	std::string	name(*_channel);

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
	_argument = true;
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
	_argument = true;
}

int List::execute()
{
	std::string list_msg;
	std::map<std::string, Channel*>::iterator it;
	serv->send_replies(_expeditor, "Channel :Users  Name", RPL_LISTSTART);
	if (!_arg)
	{
		for (it = serv->channels.begin(); it != serv->channels.end(); ++it)
		{
			// serv->send_replies(_expeditor, "<channel> <# visible> :<topic>", RPL_LIST);
			serv->send_replies(_expeditor, it->first + " <# visible> :" + it->second->getTopic() , RPL_LIST);
		}
	}
	if (serv->channels.empty() == true)
		serv->send_replies(_expeditor, ":End of /LIST", RPL_LISTEND);
	if (_arg && (serv->channels.find(*_arg) != serv->channels.end()))
	{
		it = serv->channels.find(*_arg);
		serv->send_replies(_expeditor, it->first + " <# visible> :" + it->second->getTopic() , RPL_LIST);
	}
	serv->send_replies(_expeditor, ":End of /LIST", RPL_LISTEND);

	return 0;
}


/*	PRIVMSG	*/

/*
Privmsg::Privmsg():Command()
{
	_argument = true;
	_chan = true;
}

Privmsg::~Privmsg(){}

Privmsg & Privmsg::operator=(const Privmsg & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Privmsg::Privmsg(server * serv):Command(serv)
{
	_argument = true;
	_chan = true;
}

int Privmsg::execute()
{
	if (!_arg)
	{
		serv->send_replies(_expeditor, ":No text to send", ERR_NOTEXTTOSEND);
		// serv->send_replies(_expeditor, ":No recipient given PRIVMSG", ERR_NORECIPIENT);
	}
	// if (_expeditor.getIsOperator() == false)
	// 	serv->send_replies(_expeditor, "<mask> :Wildcard in toplevel domain", ERR_WILDTOPLEVEL);

	// if (_arg->compare(serv->getName()) != 0)
	// 	serv->send_replies(_expeditor, serv->getName() + ":No such server", ERR_NOSUCHSERVER);
	// else
	// {
	// 	std::string reply = (":" + _expeditor->getUsername() + " PONG " + serv->getName() + " \r\n");
	// 	send(_expeditor->getSock(), reply.c_str(), reply.length(), 0);
	// 	return 0;
	// }
	return 0;
}
*/


/*	PING	*/

Ping::Ping():Command()
{
	_argument = true;
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
	_argument = true;
}

int Ping::execute()
{
	if (!_arg)
		serv->send_replies(_expeditor, ":No origin specified", ERR_NOORIGIN);
	if (_arg->compare(serv->getName()) != 0)
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
	_argument = true;
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
	_argument = true;
}

int Quit::execute()
{
	if (!_arg)
	{

		serv->send_replies(_expeditor, "QUIT :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 0;
	}
	// serv->user_leave_chan(_expeditor, _expeditor->getLocation(), true);
	// Deja auto gerer, rajouter possibilite de mettre un message de depart, voir dans le parsing
	return 0;
}


/*	PART	*/


Part::Part():Command()
{
	_argument = true;
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
	_argument = true;
}

int Part::execute()
{
	//PARSING Issue : [PART #a :WeeChat 2.8] 
	if (!_arg)
	{
		serv->send_replies(_expeditor, "PART :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 0;
	}
	if (_expeditor->getLocation() != *_arg)
	{

		serv->send_replies(_expeditor, *_arg + " :You're not on that channel", ERR_NOTONCHANNEL);
		return 0;
	}
	if (serv->channels.find(*_arg) != serv->channels.end())
	{

		serv->send_replies(_expeditor, *_arg + ":No such channel", ERR_NOSUCHCHANNEL);
		return 0;
	}		
	serv->user_leave_chan(_expeditor, _expeditor->getLocation(), false);
	return 0;
}