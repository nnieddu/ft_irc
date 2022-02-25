
#include "Commands.hpp"
#include "Server.hpp"

Commands::Commands(server * serv): _serv(serv)
{
	cmds_list["EXIT"] = (ptr = &Commands::exit);
	cmds_list["JOIN"] = (ptr = &Commands::join);
}

Commands::~Commands() {}

void Commands::launch(user & usr) {
	if (!_is_complete(usr.buf))						// /!\	voir si il est possible d avoir n_command > 1 dans usr.buf
		return;

	std::string cmd = parseCmds(usr.buf);
	std::map<std::string, ft_ptr>::iterator it = cmds_list.find(cmd);

	std::cout << cmd << " was found" << std::endl << std::endl;

	if (it != cmds_list.end())
	{
			std::string cmd_arg = parseCmdsArgs(usr.buf);
			std::cout << "arg is :" << cmd_arg << std::endl << std::endl;
			ptr = it->second;
			(this->*ptr)(usr, cmd_arg);
	}
	usr.clearbuf();						// /!\	voir si il est possible d avoir n_command > 1 dans usr.buf
}

void Commands::listCommands()
{
	for (std::map<std::string, ft_ptr>::iterator it = cmds_list.begin(); it!=cmds_list.end(); ++it)
		std::cout << it->first << std::endl;
}

std::string Commands::parseCmds(std::string cmd)
{
	std::string::iterator it = cmd.begin();
	while (*it != ' ' && it != cmd.end())
		it++;
	cmd.erase(it, cmd.end());
	return (cmd);
}

std::string Commands::parseCmdsArgs(std::string cmd)
{
	std::string::iterator it = cmd.begin();
	while (*it != ' ')
		it++;
	cmd.erase(cmd.begin(), it);
	return (cmd);
}

bool	Commands::_is_complete(std::string & cmd) const
{
	std::string::reverse_iterator	rit(cmd.rbegin());

	while (rit != cmd.rend() && *rit != '\n' && *rit != '\r')
		rit++;
	return (rit != cmd.rend());
}

void Commands::exit(user & usr, std::string cmd)
{
	throw(std::runtime_error("lol"));
	(void)usr;
	(void)cmd;
}

void Commands::join(user & usr, std::string arg)
{
	std::cout << "Channel : " << arg << " created, welcome !";
	if (!_serv->is_channel_member(usr, arg))
		_serv->create_channel(arg, usr);
}