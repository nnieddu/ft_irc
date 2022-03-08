#include "../incs/Interpret.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"
#include "../incs/Command.hpp"
#include "../incs/Commands.hpp"

#include <stdlib.h>

/*----------------------------------------------------------------------------*/

Interpret::Interpret(server * serv): _serv(serv)
{
	_cmds_list["pass"] = new Pass(_serv);
	_cmds_list["cap"] = new Cap(_serv);
	_cmds_list["nick"] = new Nick(_serv);
	_cmds_list["user"] = new User(_serv);
	_cmds_list["oper"] = new Oper(_serv);
	_cmds_list["quit"] = new Quit(_serv);
	_cmds_list["join"] = new Join(_serv);
	_cmds_list["part"] = new Part(_serv);
	_cmds_list["mode"] = new Mode(_serv);
	_cmds_list["topic"] = new Topic(_serv);
	_cmds_list["names"] = new Names(_serv); //
	_cmds_list["list"] = new List(_serv);
	_cmds_list["invite"] = new Invite(_serv);
	_cmds_list["kick"] = new Kick(_serv);
	_cmds_list["version"] = new Version(_serv);
	_cmds_list["time"] = new Time(_serv);
	_cmds_list["info"] = new Info(_serv);
	_cmds_list["privmsg"] = new Privmsg(_serv);
	_cmds_list["notice"] = new Notice(_serv);
	_cmds_list["whois"] = new Whois(_serv);
	_cmds_list["ping"] = new Ping(_serv);
	_cmds_list["pong"] = new Pong(_serv);
	// _cmds_list["who"] = new Who(_serv);
	//// _cmds_list["stats"] = Stat(_serv);
	//// _cmds_list["admin"] = Admin(_serv); // maybe useless
	//// _cmds_list["whowas"] = Whowas(_serv); // a voir mais relou
	//// _cmds_list["kill"] = Kill(_serv);
}

/*----------------------------------------------------------------------------*/

Interpret::~Interpret()
{
	for (std::map<std::string, Command *>::iterator	it(_cmds_list.begin()); it != _cmds_list.end(); it++)
		delete it->second;
}

/*----------------------------------------------------------------------------*/

void Interpret::treat_user_buffer(user & usr)
{
	std::vector<std::string *>*	args;

	args = parseCmds(&usr.buf);

	while (!args->empty())
	{
		if (_cmds_list.find(nameCaseIns(*args->front())) != _cmds_list.end())
			cmd_found(_cmds_list[*args->front()], usr, args);
		else
			cmd_not_found(usr, args);
	}

	delete args;
	return;
}

/*----------------------------------------------------------------------------*/

std::vector<std::string *>* Interpret::parseCmds(std::string * buf) const
{
	std::vector<std::string *>*	args = new std::vector<std::string *>;
	bool	isMode = false;

	while (hasEOC(buf))
	{
		buf = GetNextWord(buf);

		if (buf->front() != ':' && (!isMode || args->size() < 3))
			args->push_back(parseWord(buf));
		else
			args->push_back(parseMessage(buf));
		if (args->size() == 1)
		{
			std::string	tmp(nameCaseIns(*args->front()));

			if (!tmp.compare("mode"))
				isMode = true;
		}
	}
	return (args);
}

/*----------------------------------------------------------------------------*/

std::string *	Interpret::GetNextWord(std::string * buf) const
{
	std::string::iterator	it(buf->begin());

	while (*it == ' ' || *it == 7)
		it++;
	buf->erase(buf->begin(), it);
	if (buf->empty())
		return NULL;
	return buf;
}

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

void	Interpret::cmd_found(Command* cmd, user & usr, std::vector<std::string *>* args)
{
	cmd->setArgs(&usr, args);
	cmd->execute();
	cmd->reset();
	clearLeftover(args);
	return;
}

void	Interpret::cmd_not_found(user & usr, std::vector<std::string *>* args) const
{
	std::cerr << *args->front() << ": Command not found" << std::endl;

	clearLeftover(args);
	return;
}

/*----------------------------------------------------------------------------*/

bool	Interpret::hasEOC(const std::string * buf) const
{
	return (buf->find('\n') != std::string::npos || buf->find('\r') != std::string::npos);
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

/*----------------------------------------------------------------------------*/

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
