#pragma once

#include "Command.hpp"

class Privmsg : public Command
{
	public:

		Privmsg();
		virtual ~Privmsg();
		Privmsg&	operator=(const Privmsg & x);
		Privmsg(server * serv);
		virtual int	execute();
};
