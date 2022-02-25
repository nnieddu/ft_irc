
#include "Commands.hpp"
#include "Socket.hpp"
#include <stdlib.h>

Commands::Commands() 
{
	cmds_list["/exit\n"] = (ptr = &Commands::exit);
	// cmds_list["/join\n"] = (ptr = &Commands::join);
}

Commands::~Commands() {}

void Commands::isCommands(std::string cmd)
{
	for (std::map<std::string, ft_ptr>::iterator it = cmds_list.begin(); it!=cmds_list.end(); ++it)
	{
		if (it->first == cmd)
		{
			ptr = it->second;
			(this->*ptr)();
		}
	}
}

void Commands::listCommands()
{
  for (std::map<std::string, ft_ptr>::iterator it = cmds_list.begin(); it!=cmds_list.end(); ++it)
    std::cout << it->first << std::endl;
}

void Commands::exit()
{
	throw(std::runtime_error("EXIT"));
}

void Commands::join(std::string channel_name)
{
	std::cout << "Channel : " << channel_name << " created, welcome !";
}