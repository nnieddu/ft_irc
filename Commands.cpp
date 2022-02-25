
#include "Commands.hpp"
#include "Socket.hpp"
#include <stdlib.h>

Commands::Commands() 
{
	// cmds_list["/exit"] = (ptr = &Commands::exit);
	cmds_list["/join"] = (ptr = &Commands::join);
}

Commands::~Commands() {}

void Commands::launch(std::string full_cmd)
{
	std::string cmd = parseCmds(full_cmd);
	std::map<std::string, ft_ptr>::iterator it = cmds_list.find(cmd);

	if(it != cmds_list.end())
	{
			std::string cmd_arg = parseCmdsArgs(full_cmd);
			ptr = it->second;
			(this->*ptr)(cmd_arg);
	}
}

void Commands::listCommands()
{
	for (std::map<std::string, ft_ptr>::iterator it = cmds_list.begin(); it!=cmds_list.end(); ++it)
		std::cout << it->first << std::endl;
}

std::string Commands::parseCmds(std::string cmd)
{
	std::string::iterator it = cmd.begin();
	while (*it != ' ')
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

void Commands::exit()
{
	// throw(std::runtime_error("EXIT"));
}

void Commands::join(std::string cmd)
{
	std::cout << "Channel : " << cmd << " created, welcome !";
}