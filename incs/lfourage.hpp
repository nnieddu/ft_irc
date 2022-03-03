#pragma once

#include "Command.hpp"

class Names : public Command
{
	public:

		Names();
		virtual ~Names();
		Names&	operator=(const Names & x);
		Names(server * serv);
		virtual int	execute();
};