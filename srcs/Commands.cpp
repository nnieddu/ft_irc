
#include "../incs/Commands.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"

#include <stdlib.h>

Commands::Commands(server * serv): _serv(serv)
{
	cmds_list["PASS"] = (ptr = &Commands::pass);
	cmds_list["NICK"] = (ptr = &Commands::nick);
	cmds_list["USER"] = (ptr = &Commands::user_cmd);
	// cmds_list["OPER"] = (ptr = &Commands::oper);
	// cmds_list["QUIT"] = (ptr = &Commands::quit);
	cmds_list["JOIN"] = (ptr = &Commands::join);
	// cmds_list["PART"] = (ptr = &Commands::part);
	// cmds_list["MODE"] = (ptr = &Commands::mode);
	// cmds_list["TOPIC"] = (ptr = &Commands::topic);
	// cmds_list["NAMES"] = (ptr = &Commands::names);
	cmds_list["LIST"] = (ptr = &Commands::list);
	// cmds_list["INVITE"] = (ptr = &Commands::invite);
	// cmds_list["KICK"] = (ptr = &Commands::kick);
	// cmds_list["VERSION"] = (ptr = &Commands::version);
	// cmds_list["STATS"] = (ptr = &Commands::stats); // a voir
	// cmds_list["TIME"] = (ptr = &Commands::time);
	// cmds_list["ADMIN"] = (ptr = &Commands::admin); // maybe useless
	// cmds_list["INFO"] = (ptr = &Commands::info); 
	// cmds_list["PRIVMSG"] = (ptr = &Commands::privmsg);
	// cmds_list["NOTICE"] = (ptr = &Commands::notice);
	// cmds_list["WHO"] = (ptr = &Commands::who);
	// cmds_list["WHOIS"] = (ptr = &Commands::whois);
	// cmds_list["WHOWAS"] = (ptr = &Commands::whowas); // a voir mais relou
	// cmds_list["KILL"] = (ptr = &Commands::kill);
	// cmds_list["PING"] = (ptr = &Commands::ping);
	// cmds_list["PONG"] = (ptr = &Commands::pong);
}

Commands::~Commands() {}

int Commands::launch(user & usr)
{
	int ret = 0;
	std::string cmd_to_launch;
	std::string args_to_launch;
	std::string parsed_arg;
	std::map<std::string, ft_ptr>::iterator it;
	
	while ((usr.buf.find('\r') && ret == 0))
	{
		cmd_to_launch = parseCmds(usr.buf);
		if ((it = cmds_list.find(cmd_to_launch)) != cmds_list.end())
		{
			usr.buf.erase(usr.buf.find(cmd_to_launch), cmd_to_launch.size());
			if (usr.buf.empty() == false)
			{
				if (*usr.buf.begin() == ' ')
					usr.buf.erase(usr.buf.begin(), usr.buf.begin()+1);
				parsed_arg = parseCmdArgs(usr.buf);
				usr.buf.erase(usr.buf.find(parsed_arg), parsed_arg.size());
			}
			ptr = it->second;
			ret = (this->*ptr)(&usr, parsed_arg);
		}
		else
			break;
	}
	// std::cout << "Command not found" << std::endl;
	 // send replies cmd not found ou envoyer msg dans chan
	return ret;
}

std::string Commands::parseCmds(std::string cmd)
{
	std::string::iterator it = cmd.begin();
	while (*it != ' ' && it != cmd.end())
		it++;
	cmd.erase(it, cmd.end());
	return (cmd);
}

std::string Commands::parseCmdArgs(std::string arg)
{
	if (arg.find("\r") != std::string::npos)
		arg.erase(arg.find("\r"));
	if (arg.find("\n") != std::string::npos)
		arg.erase(arg.find("\n"));
	return (arg);
}

bool	Commands::_is_complete(std::string & cmd) const
{
	std::string::reverse_iterator	rit(cmd.rbegin());

	while (rit != cmd.rend() && *rit != '\n')
		rit++;
	return (rit != cmd.rend());
}

/* -- BEGIN OF COMMANDS FUNCTIONS -- */

int Commands::pass(user * usr, std::string arg)
{
	// if (usr->isRegister == true) // voir si on garde un historic pas sur de capter voir 4.1.1
	// 	 _serv->send_replies(usr, NULL, ERR_ALREADYREGISTERED);
	if (arg.empty() == true)
	{
		_serv->send_replies(usr, "You need a pass to pass bro", ERR_NEEDMOREPARAMS);
		return -1;
	}
	else if (arg != _serv->getPassword())
	{
		_serv->send_replies(usr, "WTF un intrus", ERR_PASSWDMISMATCH);
		usr->setLogged(false);
		return -1;
	}
	if (usr->getisLogged() == false)
	{
		usr->setPassword(arg);
		usr->setLogged(true);
	}
	return 0;
}

int Commands::nick(user * usr, std::string arg)
{
	if (arg.empty() == true)
		_serv->send_replies(usr, NULL, ERR_NONICKNAMEGIVEN);
	if (usr->getNickname().empty() == true)
	{
		if (_serv->isIn(arg) == false)
		{
			usr->setNickname(arg);
			return 0;
		}
		else
			_serv->send_replies(usr, NULL, ERR_NICKNAMEINUSE);
	}
	// ERR_ERRONEUSNICKNAME if non conforme 'anonymous' ou char spe voir grammar protocol d'apres rfc
	// mais pas de categorie grammar protocol dans la rfc..lul

	// ERR_NICKCOLLISION osef ?
	return 1;
}

int Commands::user_cmd(user * usr, std::string arg)
{
//  Parameters: <username> <hostname> <servername> <realname>
	if (arg.empty() == true) // todo: si pas asse d'arg, a voir avec parsing
	{
		_serv->send_replies(usr, NULL, ERR_NEEDMOREPARAMS);
		return 1;
	}
	return 0;
}

// void Commands::oper(user * usr, std::string arg)
// {
// }

// void Commands::quit(user * usr, std::string arg)
// {
// }

int Commands::join(user * usr, std::string arg)
{
	if (arg.empty() == true)
	{
		_serv->send_replies(usr, "JOIN need name parameter man", ERR_NEEDMOREPARAMS);
		return 1;
	}
	if (!usr->isMember(arg) && _serv->_channels.find(arg) == _serv->_channels.end() 
		&& usr->getisLogged() == true)
	{
		std::cout << "Channel : " << arg << " created" << std::endl;
		//usr->setOperator(true);
		_serv->create_channel(*usr, arg);
	}
	else
		usr->setLocation(arg);	// /!\ locations related stuff
	return 0;
}

// void Commands::mode(user * usr, std::string arg)
// {
// }

// void Commands::topic(user * usr, std::string arg)
// {
// }

// void Commands::names(user * usr, std::string arg)
// {
// }

int Commands::list(user * usr, std::string arg)
{
	(void)usr;
	for (std::map<std::string, std::vector<user*> >::iterator it = _serv->_channels.begin();
		it != _serv->_channels.end(); ++it)
	{
		std::cout << it->first << std::endl;
	}
	std::cout << "EOF List" << std::endl;
	return 0;
}

// void Commands::invite(user * usr, std::string cmd)
// {
// }

// void Commands::kick(user * usr, std::string arg)
// {
// }