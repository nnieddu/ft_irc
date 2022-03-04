//
// Created by Maximilien Rozniecki on 3/1/22.
//

#pragma once

#include "Command.hpp"

class Topic : public Command {
	public:

		Topic();
		Topic(server*);
		virtual ~Topic();
		Topic&	operator=(const Topic&);
		virtual void execute();
};
