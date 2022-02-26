
#include "Commands.hpp"
#include "Socket.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"

#include <stdlib.h>

void	ft_bzero(void *s, size_t n)
{
    unsigned char	*str;
    size_t			i;

    i = 0;
    str = (unsigned char *)s;
    while (i < n)
    {
        *str = 0;
        str++;
        i++;
    }
}

Commands::Commands(server * serv): _serv(serv)
{
	cmds_list["EXIT"] = (ptr = &Commands::exit);
	cmds_list["JOIN"] = (ptr = &Commands::join);
	cmds_list["LIST"] = (ptr = &Commands::list);
	cmds_list["KICK"] = (ptr = &Commands::kick);
}

Commands::~Commands() {}

void Commands::launch(user & usr)
{
	// if (!_is_complete(usr.buf))			// /!\	voir si il est possible d avoir n_command > 1 dans usr.buf
	// 	return;

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

	while (rit != cmd.rend() && *rit != '\n') //&& *rit !=)
		rit++;
	return (rit != cmd.rend());
}


void Commands::exit(user * usr, std::string cmd)
{
	(void)usr;
	(void)cmd;
	throw(std::runtime_error("EXIT"));
}

void Commands::join(user * usr, std::string arg)
{
	std::cout << "Channel : " << arg;

	if (!usr->isMember(arg))
	{
		usr->setOperator(true);
		_serv->create_channel(*usr, arg);
	}
	else
		usr->setLocation(arg);		// /!\ locations related stuff
}

void Commands::list(user * usr, std::string arg)
{
	(void)usr;
	std::cout << "Je passe par list:" << arg << std::endl;
	for (std::map<std::string, std::vector<user*> >::iterator it = _serv->getChans()->begin(); 
		it != _serv->getChans()->end(); ++it)
	{
		std::cout << it->first;
	}
}

void Commands::kick(user * usr, std::string arg)
{
	(void)usr;
	std::cout << "Je passe par kick:" << arg << std::endl;
// 	if (arg.empty() == true)
// 	{
// 		_serv->send_reply(usr, ERR_NEEDMOREPARAMS);
// 		return ;
// 	}
}