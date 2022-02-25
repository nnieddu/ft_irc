
#include "Commands.hpp"
#include "Socket.hpp"
#include <stdlib.h>

Commands::Commands() 
{
	cmds_list["/exit\n"] = (p = &Commands::exit);
	// cmds_list["/join\n"] = (p = &Commands::join);
}

Commands::~Commands() {}

void Commands::isCommands(std::string cmd)
{
	for (std::map<std::string, ft_ptr>::iterator it = cmds_list.begin(); it!=cmds_list.end(); ++it)
	{
		if (it->first == cmd)
		{
			p = it->second;
			(this->*p)();
		}
	}
}

void Commands::listCommands()
{
  for (std::map<std::string, ft_ptr>::iterator it = cmds_list.begin(); it!=cmds_list.end(); ++it)
    std::cout << it->first << '\n';
}

void Commands::exit()
{
	throw(std::runtime_error("EXIT"));
}

void Commands::join(std::string channel_name)
{
	std::cout << "Channel : " << channel_name << " created, welcome !";
}