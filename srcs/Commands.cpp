
#include "../incs/Commands.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"

#include <stdlib.h>

Command::Command() {}//: serv(NULL), _channel(NULL), _user(NULL), _arg(NULL), _chan(false), _usr(false), _argument(false){}

Command &	Command::operator=(const Command & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Command::~Command()
{
	_user = NULL;
	if (_channel)
		delete _channel;
	if (_arg)
		delete _arg;
}

Command::Command(server * serv): serv(serv), _channel(NULL), _user(NULL), _arg(NULL), _chan(false), _usr(false), _argument(false)
{}

int	Command::execute() { return 0; }

void	Command::setExpeditor(user * expeditor)
{
	_expeditor = expeditor;
}

void	Command::setArgs(std::vector<std::string> args)
{
	if (args.empty())
		return ;
	else if (args.size() == 2)
	{
		_channel = new std::string(args[0]);
		_arg = new std::string(args[1]);
	}
	else
		_arg = new std::string(args[0]);
}

int	Command::getReply() const{ return _reply; }

bool	Command::needChannel() const{ return _chan; }

bool	Command::needUser() const{ return _usr; }

bool	Command::needArg() const{ return _argument; }

void	Command::reset()
{
	_user = NULL;
	if (_channel)
		delete _channel;
	_channel = NULL;
	if (_arg)
		delete _arg;
	_arg = NULL;
}

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
	if (*_arg == "") //
	{
		serv->send_replies(_expeditor, "PASS :Not enough parameters", ERR_NEEDMOREPARAMS);
		return -1;
	}
	else if (_arg->compare(serv->getPassword()) != 0)
	{
		serv->send_replies(_expeditor, ":Password incorrect", ERR_PASSWDMISMATCH); // pas dans la rfc au log
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

/*	USER	*/

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
// weechat le gere tt seul mais on doit check quand l'user input ya bien '&' or '#' et pas cree le chan si non
	std::string	name(*_arg);

	if (!_expeditor->isMember(name) && serv->_channels.find(name) == serv->_channels.end() 
		&& _expeditor->getisLogged() == true)
	{
		std::cout << "Channel : " << name << " created" << std::endl;
		//usr->setOperator(true);
		serv->create_channel(*_expeditor, name);
	}
	else
		_expeditor->setLocation(name);	// /!\ locations related stuff
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

//            ERR_NOSUCHSERVER                RPL_LISTSTART
//            RPL_LIST                        RPL_LISTEND