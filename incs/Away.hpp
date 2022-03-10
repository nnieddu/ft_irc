//
// Created by Maximilien Rozniecki on 3/10/22.
//

#pragma once

#include "Command.hpp"
#include "Server.hpp"

class Away : public Command {
public:

	Away();
	Away(server*);
	virtual ~Away();
	Away&	operator=(const Away&);
	virtual void execute();
};
