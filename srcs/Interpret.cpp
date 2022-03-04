
#include "../incs/Interpret.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"
#include "../incs/Command.hpp"
#include "../incs/Commands.hpp"

#include <stdlib.h>

Interpret::Interpret(server * serv): _serv(serv)
{
	cmds_list["pass"] = new Pass(_serv);
	cmds_list["nick"] = new Nick(_serv);
	cmds_list["user"] = new User(_serv);
	// cmds_list["oper"] = Oper(_serv);
	cmds_list["quit"] = new Quit(_serv);
	cmds_list["join"] = new Join(_serv);
	cmds_list["part"] = new Part(_serv);
	// cmds_list["mode"] = Mode(_serv);
	cmds_list["topic"] = new Topic(_serv);
	cmds_list["names"] = new Names(_serv); //
	cmds_list["list"] = new List(_serv);
	// cmds_list["invite"] = Invite(_serv);
	// cmds_list["kick"] = Kick(_serv);
	cmds_list["version"] = new Version(_serv);
	// cmds_list["time"] = Time(_serv);
	cmds_list["info"] = new Info(_serv);
	cmds_list["privmsg"] = new Privmsg(_serv);
	// cmds_list["notice"] = Notice(_serv);
	cmds_list["whois"] = new Whois(_serv);
	cmds_list["ping"] = new Ping(_serv);
	// cmds_list["who"] = new Who(_serv);
	//// cmds_list["stats"] = Stat(_serv);
	//// cmds_list["admin"] = Admin(_serv); // maybe useless
	//// cmds_list["whowas"] = Whowas(_serv); // a voir mais relou
	//// cmds_list["kill"] = Kill(_serv);
	// cmds_list["pong"] = Pong(_serv);
}

Interpret::~Interpret()
{
	for (std::map<std::string, Command *>::iterator	it(cmds_list.begin()); it != cmds_list.end(); it++)
		delete it->second;
}

bool	Interpret::hasEOC(const std::string * buf) const
{
	return (buf->find('\n') != std::string::npos || buf->find('\r') != std::string::npos);
}

int Interpret::launch(user & usr)
{
	std::vector<std::string *>*	args;
	int							ret;

	args = parseCmds(&usr.buf);

	while (!args->empty())
	{
		if (cmds_list.find(nameCaseIns(*args->front())) != cmds_list.end())
			ret = cmd_found(cmds_list[*args->front()], usr, args);
		else
			ret = cmd_not_found(usr, args);
	}

	delete args;
	return ret;
}

int	Interpret::cmd_found(Command* cmd, user & usr, std::vector<std::string *>* args)
{
	cmd->setExpeditor(&usr);
	cmd->setArgs(args);
	cmd->execute();
	cmd->reset();
	clearLeftover(args);
	return 0;
}

int	Interpret::cmd_not_found(user & usr, std::vector<std::string *>* args) const
{
	std::cerr << *args->front() << ": Command not found" << std::endl;

	clearLeftover(args);
	return 1;
}

std::vector<std::string *>* Interpret::parseCmds(std::string * buf) const
{
	std::vector<std::string *>*	args = new std::vector<std::string *>;

	while (hasEOC(buf))
	{
		buf = GetNextWord(buf);

		if (buf->front() != ':')
			args->push_back(parseWord(buf));
		else
			args->push_back(parseMessage(buf));
	}
	return (args);
}

std::string *	Interpret::GetNextWord(std::string * buf) const
{
	std::string::iterator	it(buf->begin());

	while (*it == ' ')
		it++;
	buf->erase(buf->begin(), it);
	if (buf->empty())
		return NULL;
	return buf;
}

std::string *	Interpret::isEOC(std::string * buf) const
{
	std::string::iterator	it(buf->begin());
	std::string				arg;

	if (*(++it) == '\n')
		++it;
	arg.assign(buf->begin(), it);
	buf->erase(buf->begin(), it);

	return new std::string(arg);
}

std::string *	Interpret::parseWord(std::string * buf) const
{
	std::string::iterator	it(buf->begin());
	std::string				arg;

	if (*it == '\r' || *it == '\n')
		return isEOC(buf);
	while(it != buf->end() && *it != ' ' && *it != 7 && *it != '\r' && *it != '\n')
		it++;
	arg.assign(buf->begin(), it);
	buf->erase(buf->begin(), it);

	return new std::string(arg);
}

std::string *	Interpret::parseMessage(std::string * buf) const
{
	std::string::iterator	it(buf->begin());
	std::string				arg;

	if (*it == '\r' || *it == '\n')
		return isEOC(buf);
	while(it != buf->end() && *it != '\r' && *it != '\n')
		it++;
	arg.assign(buf->begin(), it);
	buf->erase(buf->begin(), it);

	return new std::string(arg);
}

void	Interpret::clearLeftover(std::vector<std::string *>* args) const
{
	std::vector<std::string *>::iterator	it(args->begin());

	while (!hasEOC(*it))
	{
		delete *it;
		args->erase(it);
		it = args->begin();
	}
	delete *it;
	args->erase(it);
	return ;
}
