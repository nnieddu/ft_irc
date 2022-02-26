
#include "../incs/Commands.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"

#include <stdlib.h>

Commands::Commands(server * serv): _serv(serv)
{
	cmds_list["PASS"] = (ptr = &Commands::pass);
	cmds_list["NICK"] = (ptr = &Commands::nick);
	// cmds_list["USER"] = (ptr = &Commands::user);
	// cmds_list["OPER"] = (ptr = &Commands::oper);
	// cmds_list["QUIT"] = (ptr = &Commands::quit);
	cmds_list["JOIN"] = (ptr = &Commands::join);
	// cmds_list["PART"] = (ptr = &Commands::part);
	// cmds_list["MODE"] = (ptr = &Commands::mode);
	// cmds_list["TOPIC"] = (ptr = &Commands::topic);
	// cmds_list["NAMES"] = (ptr = &Commands::names);
	cmds_list["LIST"] = (ptr = &Commands::list);
	// cmds_list["INVITE"] = (ptr = &Commands::invite);
	cmds_list["KICK"] = (ptr = &Commands::kick);
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

void Commands::launch(user & usr)
{
	std::string cmd = parseCmds(usr.buf);
	std::map<std::string, ft_ptr>::iterator it = cmds_list.find(cmd);

	if(it != cmds_list.end())
	{
		std::string cmd_arg = parseCmdsArgs(usr.buf);
		ptr = it->second;
		(this->*ptr)(&usr, cmd_arg);
	}
}

std::string Commands::parseCmds(std::string cmd)
{
	std::string::iterator it = cmd.begin();
	while (*it != ' ' && it != cmd.end())
		it++;
	cmd.erase(it, cmd.end());
	return (cmd);
}

std::string Commands::parseCmdsArgs(std::string cmd)
{
	std::string::iterator it = cmd.begin();
	while (*it != ' ')
		it++;
	cmd.erase(cmd.begin(), it);
	return (cmd);
}

bool	Commands::_is_complete(std::string & cmd) const
{
	std::string::reverse_iterator	rit(cmd.rbegin());

	while (rit != cmd.rend() && *rit != '\n')
		rit++;
	return (rit != cmd.rend());
}

/* -- BEGIN OF COMMANDS FUNCTIONS -- */

void Commands::pass(user * usr, std::string arg)
{
	// if (usr->isRegister == true) // voir si on garde un historic pas sur de capter voir 4.1.1
	// 	 _serv->send_replies(usr, NULL, ERR_ALREADYREGISTERED);
	if (arg.empty() == true)
		_serv->send_replies(usr, NULL, ERR_NEEDMOREPARAMS);
	else
		usr.setPassword(arg);	
}

void Commands::nick(user * usr, std::string arg)
{
	if (usr->getNickname().empty() == true)
		usr->setNickname(arg);
}

// void Commands::user(user * usr, std::string arg)
// {
// }

// void Commands::oper(user * usr, std::string arg)
// {
// }

// void Commands::quit(user * usr, std::string arg)
// {
// }

void Commands::join(user * usr, std::string arg)
{
	std::cout << "Channel : " << arg;

	if (!usr->isMember(arg))
	{
		//usr->setOperator(true);
		_serv->create_channel(*usr, arg);
	}
	else
		usr->setLocation(arg);	// /!\ locations related stuff
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

void Commands::list(user * usr, std::string arg)
{
	(void)usr;
	std::cout << "Je passe par list:" << arg << std::endl;
	for (std::map<std::string, std::vector<user*> >::iterator it = _serv->_channels.begin(); 
		it != _serv->_channels.end(); ++it)
	{
		std::cout << it->first;
	}
}

// void Commands::invite(user * usr, std::string cmd)
// {
// }

void Commands::kick(user * usr, std::string arg)
{
	(void)usr;
	std::cout << "Je passe par kick:" << arg << std::endl;
// 	if (arg.empty() == true)
// 	{
		// _serv->send_replies(usr, ERR_NEEDMOREPARAMS); // a retourner quand ne colle pas avec le nombre d'arg pr la cmd
// 		return ;
// 	}
}