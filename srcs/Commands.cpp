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

/*	USER	*/ // Executee seulement lors de la premiere co

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
	if (!_arg)
	{

		serv->send_replies(_expeditor, "USER :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 0;
	}
	// if (_expeditor->getisLogged() == true)
	// 	serv->send_replies(_expeditor, ":You may not reregister", ERR_ALREADYREGISTRED);
	// if (_expeditor->getisLogged() == true) // le parsing bloque
	// {
	// 	std::cout << "USERGOOD\n";

	// 	std::string prefix = ":" + _expeditor->getUsername();
	// 	std::string logged = prefix +  " " + RPL_WELCOME + " " + _expeditor->getUsername() + \
	// 		" Welcome to the Internet Relay Network :" + _expeditor->getUsername() + "\r\n";
	// 	send(_expeditor->getSock(), logged.c_str(), logged.length(), 0);		
	// }
	return 0;
}

/*	JOIN	*/

Join::Join():Command()
{
	_argument = true;
	// _chan = true;
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
	_argument = true;
	// _chan = true;
}


int	Join::execute()
{
	if (!_arg)
	{
		serv->send_replies(_expeditor, "JOIN :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 1;
	}
	std::string	name(*_arg);
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
	_chan = true;
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
	_chan = true;
}

int List::execute()
{
	for (std::map<std::string, Channel*>::iterator it = serv->channels.begin();
		it != serv->channels.end(); ++it)
	{
		std::cout << it->first << std::endl;
	}
	
	std::cout << "EOF List" << std::endl;
	return 0;
}
//    Parameters: [<channel>{,<channel>} [<server>]]

//    The list message is used to list channels and their topics.  If  the
//    <channel>  parameter  is  used,  only  the  status  of  that  channel
//    is displayed.  Private  channels  are  listed  (without  their
//    topics)  as channel "Prv" unless the client generating the query is
//    actually on that channel.  Likewise, secret channels are not listed
//    at  all  unless  the client is a member of the channel in question.

//    Numeric Replies:
//    ERR_NOSUCHSERVER                RPL_LISTSTART
//    RPL_LIST                        RPL_LISTEND



/*	PRIVMSG	*/   // NOTICE
//  Command: PRIVMSG Parameters: <receiver>{,<receiver>} <text to be sent>
//   PRIVMSG is used to send private messages between users.  <receiver>
//    is the nickname of the receiver of the message.  <receiver> can also
//    be a list of names or channels separated with commas.
//    ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
//    ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
//    ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//    ERR_NOSUCHNICK
//    RPL_AWAY

//    The <receiver> parameter may also me a host mask  (#mask) In  both cases the server will only send the PRIVMSG
//    to those who have a server or host matching the mask.  The mask  must have at  least  1  (one)  "."  
//    in it and no wildcards following the last ".".  This requirement exists to prevent people sending messages
//    to  "#*"  or "$*",  which  would  broadcast  to  all  users; from experience, this is abused more than 
//    used responsibly and properly. Wildcards are  the  '*' and  '?'   characters.   This  extension  to
//    the PRIVMSG command is only available to Operators.

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