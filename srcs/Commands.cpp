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
	_expeditor->setLogged(true);
	return 0;
	// if (usr->isRegister == true) // voir si on garde un historic pas sur de capter voir 4.1.1
	// 	 serv->send_replies(usr, NULL, ERR_ALREADYREGISTRED);
	if (!_arg) //
	{
		serv->send_replies(_expeditor, "PASS :Not enough parameters", ERR_NEEDMOREPARAMS);
		return -1;
	}
	else if (_arg->compare(serv->getPassword()) != 0)
	{
		serv->send_replies(_expeditor, ":Password incorrect", ERR_PASSWDMISMATCH); // pas dans la rfc au loggin maybe a remove
		_expeditor->setLogged(false);
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
	if (serv->isIn(*_arg) == false)
	{
		std::string str = ":" + _expeditor->getNickname() + " NICK " + ":" + *_arg + "\r\n";
	    send(_expeditor->getSock(), str.c_str(), strlen(str.c_str()), 0);
		_expeditor->setNickname(*_arg);
		return 0;
	}
	// else
	// 	serv->send_replies(_expeditor, NULL, ERR_NICKNAMEINUSE); // CAUSE UN SEGFAULT NOW

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
	if (_expeditor->getisLogged() == true)
		serv->send_replies(_expeditor, ":You may not reregister", ERR_ALREADYREGISTRED);
	return 0;
}

/*	JOIN	*/

Join::Join():Command()
{
	_argument = true;
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
}


int	Join::execute()
{
	if (!_arg)
	{
		serv->send_replies(_expeditor, "JOIN :Not enough parameters", ERR_NEEDMOREPARAMS);
		return 1;
	}
	// std::string	name("#" + *_arg);
// weechat le gere tt seul mais on doit check quand l'user input des chan si ya bien '&' or '#' et pas cree le chan si non
	std::string	name(*_arg);

	if (!_expeditor->isMember(name) && serv->_channels.find(name) == serv->_channels.end() 
		&& _expeditor->getisLogged() == true) //
	{
		std::cout << "Channel : " << name << " created" << std::endl;
		// _expeditor->setOperator(true);
		serv->create_channel(*_expeditor, name);
	}
	else
	{
		std::string replies = ":" + _expeditor->getNickname() + " JOIN :" + name + "\r\n";
		send(_expeditor->getSock(), replies.c_str(), replies.length(), 0);
		_expeditor->setLocation(name);	// /!\ locations related stuff
	}
	return 0;
}
        //    ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
        //    ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
        //    ERR_CHANNELISFULL               ERR_BADCHANMASK
        //    ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
        //    RPL_TOPIC

/*	LIST	*/

List::List():Command()
{
	// _chan = true; // fait segfault si "LIST" (sans arg(sur netcat)
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
	// _chan = true; // fait segfault si "LIST" (sur netcat)
}

int List::execute()
{
	for (std::map<std::string, std::vector<user*> >::iterator it = serv->_channels.begin();
		it != serv->_channels.end(); ++it)
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