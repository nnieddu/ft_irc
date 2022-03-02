//
// Created by fenrir on 02/03/2022.
//

#pragma once

#include "Command.hpp"
#include "../incs/Server.hpp"
#include "../incs/Channel.hpp"

class Mode : public Command {

	private:
		int	modeChan(Channel&, std::string&, std::string&);
		int	modeUser(user&, std::string&, std::string&);

	public:

		Mode();
		Mode(server*);
		virtual ~Mode();
		Mode&	operator=(const Mode&);
		virtual int execute();
};
