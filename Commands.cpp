
#include "Commands.hpp"
#include "Socket.hpp"
#include <stdlib.h>

Commands::Commands() 
{
	cmds_list.push_back("/exit\n");
	cmds_list.push_back("/join\n");
}

Commands::~Commands() {}

int Commands::IsCommands(std::string cmd)
{
	for (std::vector<std::string>::iterator it = cmds_list.begin() ; it != cmds_list.end(); ++it)
	{
		if (*it == cmd)
			return 1;
	}
	return (0);
}

void Commands::listCommands()
{
	for (std::vector<std::string>::iterator it = cmds_list.begin() ; it != cmds_list.end(); ++it)
		std::cout << *it;
}

void Commands::exit()
{
	std::cout << "TEST EXIT\n";
	throw(std::runtime_error("EXIT"));
}