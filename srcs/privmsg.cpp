#include "../incs/Command.hpp"
#include "../incs/Commands.hpp"
#include "../incs/Server.hpp"
#include "../incs/User.hpp"

/*
4.4.1 Private messages

		Command: PRIVMSG
	Parameters: <receiver>{,<receiver>} <text to be sent>

	PRIVMSG is used to send private messages between users.  <receiver>
	is the nickname of the receiver of the message.  <receiver> can also
	be a list of names or channels separated with commas.

	The <receiver> parameter may also me a host mask  (#mask)  or  server
	mask  ($mask).   In  both cases the server will only send the PRIVMSG
	to those who have a server or host matching the mask.  The mask  must
	have at  least  1  (one)  "."  in it and no wildcards following the
	last ".".  This requirement exists to prevent people sending messages
	to  "#*"  or "$*",  which  would  broadcast  to  all  users; from
	experience, this is abused more than used responsibly and properly.
	Wildcards are  the  '*' and  '?'   characters.   This  extension  to
	the PRIVMSG command is only available to Operators.

   Numeric Replies:

			ERR_NORECIPIENT						ERR_NOTEXTTOSEND
			ERR_CANNOTSENDTOCHAN				ERR_NOTOPLEVEL
			ERR_WILDTOPLEVEL					ERR_TOOMANYTARGETS
			ERR_NOSUCHNICK
			RPL_AWAY

	Examples:

:Angel PRIVMSG Wiz :Hello are you receiving this message ?;
	Message from Angel to Wiz.

PRIVMSG Angel :yes I'm receiving it !receiving it !'u>(768u+1n) .br;
	Message to Angel.

PRIVMSG jto@tolsun.oulu.fi :Hello !;
	Message to a client on server tolsun.oulu.fi with username of "jto".

PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.;
	Message to everyone on a server which has a name matching *.fi.

PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions;
	Message to all users who come from a host which has a name matching *.edu.
*/

Privmsg::Privmsg():Command()
{
	_args[RECEIVER].isNeeded	= true;
	_args[MESSAGE].isNeeded	= true;
}

Privmsg::~Privmsg() {}

Privmsg & Privmsg::operator=(const Privmsg & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Privmsg::Privmsg(server * serv):Command(serv)
{
	_args[RECEIVER].isNeeded	= true;
	_args[MESSAGE].isNeeded	= true;
}

void Privmsg::execute()
{
	std::string	*			receiver = _args[RECEIVER].arg;
	std::string	*			arg = _args[MESSAGE].arg;
	std::deque<std::string>	list;

	if (!receiver)
		return serv->send_replies(_expeditor, "PRIVMSG :No receiver specified", ERR_NORECIPIENT);
	if (!arg)
		return serv->send_replies(_expeditor, "PRIVMSG :No text to send", ERR_NOTEXTTOSEND);

	list = _args[RECEIVER].parseList();

	while (!list.empty())
	{
		if (HasChannelPrefix(list[0]))
		{
			if (serv->channels.find(nameCaseIns(list[0])) != serv->channels.end())
			{
				if (serv->send_msg_to_channel(_expeditor, serv->getChannel(
						nameCaseIns(list[0])), *arg))
					serv->send_replies(_expeditor, "PRIVMSG :cannot send to channel", ERR_CANNOTSENDTOCHAN);
			}
			else if (serv->send_msg_to_user(_expeditor, serv->getUser(list[0]), *arg, ""))
				serv->send_replies(_expeditor, "PRIVMSG :No such nick", ERR_NOSUCHNICK);
		}
		else
		{
			user * usr = serv->getUser(list[0]);

			if (usr->isAway())
				serv->send_replies(_expeditor, "PRIVMSG :receiver is away", RPL_AWAY);
			else if (serv->send_msg_to_user(_expeditor, usr, *arg, ""))
				serv->send_replies(_expeditor, "PRIVMSG :No such nick", ERR_NOSUCHNICK);
		}
		list.erase(list.begin());
	}
	return ;
}

/*
4.2.5 Names message

		Command: NAMES
	Parameters: [<channel>{,<channel>}]

	By using the NAMES command, a user can list all nicknames that are
	visible to them on any channel that they can see.  Channel names
	which they can see are those which aren't private (+p) or secret (+s)
	or those which they are actually on.  The <channel> parameter
	specifies which channel(s) to return information about if valid.
	There is no error reply for bad channel names.

	If no <channel> parameter is given, a list of all channels and their
	occupants is returned.  At the end of this list, a list of users who
	are visible but either not on any channel or not on a visible channel
	are listed as being on `channel' "*".

	Numerics:

			RPL_NAMREPLY			RPL_ENDOFNAMES

	Examples:

	NAMES #twilight_zone,#42;
		list visible users on #twilight_zone and #42 if the channels are visible to you.

	NAMES;
		list all visible channels and users
*/

Names::Names():Command()
{
	_args[CHANNEL].isNeeded = true;
}

Names::~Names() {}

Names & Names::operator=(const Names & x)
{
	if (this != &x)
		serv = x.serv;
	return *this;
}

Names::Names(server * serv):Command(serv)
{
	_args[CHANNEL].isNeeded = true;
}

static std::string	getChannelNicks(Channel * chan)
{
	std::stringstream	output;

	output << "Nicks " << chan->getName();
	for (std::set<user *>::iterator	it(chan->getUsers().begin()); it != chan->getUsers().end(); it++)
	{
		output << " " << (*it)->getNickname();
	}
	output << std::endl;
	return output.str();
}

void Names::execute()
{
	std::string	*		channel = _args[CHANNEL].arg;
	std::stringstream	output;

	if (channel)
	{
		std::deque<std::string> list = _args[CHANNEL].parseList();

		while (!list.empty())
		{
			std::map<std::string, Channel*>::iterator	it(serv->channels.find(list.front()));

			if (it != serv->channels.end())
				it->second->send_names_replies(_expeditor);
			list.pop_front();
		}
	}
	else
	{
		std::string	names;

		for (std::map<std::string, Channel* >::iterator it(serv->channels.begin()); it != serv->channels.end(); it++)
			it->second->send_names_replies(_expeditor);
		for (std::vector<user*>::iterator it(serv->getUsers().begin()); it != serv->getUsers().end(); it++)
		{
			if ((*it)->getChannels().empty())
				names += (*it)->getNickname() + " ";
		}
		if (!(names.empty()))
		{
			std::string replies = ":" + _expeditor->getUsername() +  " " + RPL_NAMREPLY + " " + _expeditor->getNickname() + " " +  "= * :" + names + "\r\n";
			send(_expeditor->getSock(), replies.c_str(), replies.length(), 0);
		}
	}
	serv->send_replies(_expeditor, "* :End of names list", RPL_ENDOFNAMES);
	return ;
}
