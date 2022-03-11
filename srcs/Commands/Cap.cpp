#include "../../incs/Commands.hpp"

/*----------------------------------------------------------------------------*/

Cap::~Cap(){}

Cap & Cap::operator=(const Cap & x)
{
	if (this != &x)
		_serv = x.getServ();
	return *this;
}

Cap::Cap(server * serv):Command(serv)
{}

/*----------------------------------------------------------------------------*/

void Cap::execute(){
}