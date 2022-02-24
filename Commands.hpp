
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

class Commands
{
	private:
		Commands(const Commands& x);
		Commands& operator=(const Commands& x);
	public:
		Commands();
		~Commands();
		
		std::vector<std::string> cmds_list; // remplacer par map de ptr de ft

		int	IsCommands(std::string cmd);
		void listCommands();

		void exit();
};
