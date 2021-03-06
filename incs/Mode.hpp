//
// Created by fenrir on 02/03/2022.
//

#pragma once

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class Mode : public Command {

	private:
		void	modeChan(Channel&, std::string&, std::string&, std::string&);

		void	modeUser(user&, std::string&, std::string&, std::string&);
		static std::string	receivModeIs(Channel&) ;
		static std::string	receivModeIs(user&) ;
		void 	anonMode(bool onOff, Channel&) const;

	public:

		Mode();
		Mode(server*);
		virtual ~Mode();
		Mode&	operator=(const Mode&);
		virtual void execute();
};
