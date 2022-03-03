
#include "../incs/Interpret.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"
#include "../incs/Command.hpp"
#include "../incs/Commands.hpp"

#include <stdlib.h>

Interpret::Interpret(server * serv): _serv(serv)
{
	cmds_list["PASS"] = new Pass(_serv);
	cmds_list["NICK"] = new Nick(_serv);
	// cmds_list["USER"] = new User(_serv);
	// cmds_list["OPER"] = Oper(_serv);
	cmds_list["QUIT"] = new Quit(_serv);
	cmds_list["JOIN"] = new Join(_serv);
	cmds_list["PART"] = new Part(_serv);
	// cmds_list["MODE"] = Mode(_serv);
	cmds_list["TOPIC"] = new Topic(_serv);
	cmds_list["NAMES"] = new Names(_serv); //
	cmds_list["LIST"] = new List(_serv);
	// cmds_list["INVITE"] = Invite(_serv);
	// cmds_list["KICK"] = Kick(_serv);
	cmds_list["VERSION"] = new Version(_serv);
	// cmds_list["TIME"] = Time(_serv);
	cmds_list["INFO"] = new Info(_serv);
	cmds_list["PRIVMSG"] = new Privmsg(_serv);
	// cmds_list["NOTICE"] = Notice(_serv);
	cmds_list["WHOIS"] = new Whois(_serv);
	cmds_list["PING"] = new Ping(_serv);
	// cmds_list["WHO"] = new Who(_serv);
	//// cmds_list["STATS"] = Stat(_serv);
	//// cmds_list["ADMIN"] = Admin(_serv); // maybe useless
	//// cmds_list["WHOWAS"] = Whowas(_serv); // a voir mais relou
	//// cmds_list["KILL"] = Kill(_serv);
	// cmds_list["PONG"] = Pong(_serv);
}

Interpret::~Interpret()
{
	for (std::map<std::string, Command *>::iterator	it(cmds_list.begin()); it != cmds_list.end(); it++)
		delete it->second;
}

int Interpret::launch(user & usr)
{
	Command	*									cmd;
	std::vector<std::string *>					args;
	std::map<std::string, Command *>::iterator	it;

	int ret = 0;
	
	while (usr.buf.empty() == false && (usr.buf.find('\r') && ret == 0))
	{
		_iseoc = false;

		if ((it = cmds_list.find(parseCmds(&usr.buf))) != cmds_list.end())
		{
			cmd = it->second;
			cmd->setExpeditor(&usr);

			if (_iseoc == false && cmd->needReceiver())
				args.push_back(parseWord(&usr.buf));
			if (_iseoc == false && cmd->needNick())
				args.push_back(parseWord(&usr.buf));
			if (_iseoc == false && cmd->needChannel())
				args.push_back(parseWord(&usr.buf));
			if (_iseoc == false && cmd->needUser())
				args.push_back(parseWord(&usr.buf));
			if (_iseoc == false && cmd->needArg())
				args.push_back(parseMessage(&usr.buf));
			if (_iseoc == false && cmd->needPass())
				args.push_back(parseWord(&usr.buf));

			cmd->setArgs(args);
			ret = cmd->execute();
			cmd->reset();

			if (_iseoc == false)
				clearLeftover(&usr.buf);
		}
		else
			ret = cmd_not_found(usr);
	}
	return ret;
}

std::string Interpret::parseCmds(std::string * buf)
{
	std::string::iterator	it = buf->begin();
	std::string				cmd;
	
	it = buf->begin();
	while (it != buf->end() && *it != '\r' && *it != '\n' && *it != ' ')
		it++;
	if (it != buf->end() && *it != ' ')
		it = IsEOC(it, buf);
	cmd.assign(buf->begin(), it);
	buf->erase(buf->begin(), it);
	return (cmd);
}

int	Interpret::cmd_not_found(user & usr)
{
	std::string::iterator	it(usr.buf.begin());

	while (it != usr.buf.end() && *it != '\r' && *it != '\n')		// tel quel : fait sortir de la boucle
		it++;														// quoi qu'il arrive, peut etre
	usr.buf.erase(usr.buf.begin(), it);								// a changer si il y a plusieurs EOC
	std::cerr << "Command not found" << std::endl; // pass tt le temps par la 
	return 1;
}

std::string *	Interpret::parseWord(std::string * buf)
{
	std::string::iterator	first(buf->begin());
	std::string::iterator	last;
	std::string				arg;

	if (GetNextWord(buf) == NULL)
		return NULL;
	last = first;
	while(last != buf->end() && *last != ' ' && *last != 7 && *last != '\r' && *last != '\n')
		last++;
	if (last != buf->end() && (*last == 7 || *last == '\r' || *last == '\n'))
		last = IsEOC(last, buf);
	arg.assign(first, last);
	buf->erase(first, last);
	return new std::string(arg);
}

std::string *	Interpret::parseMessage(std::string * buf)
{
	std::string::iterator	first;
	std::string::iterator	last;
	std::string				arg;

	if (GetMessageStart(buf) == NULL)
		return NULL;
	first = buf->begin();
	last = first;
	while(last != buf->end() && *last != '\r' && *last != '\n' )
		last++;
	if (last != buf->end())
		last = IsEOC(last, buf);
	arg.assign(first, last);
	buf->erase(first, last);
	return new std::string(arg);
}

std::string *	Interpret::GetMessageStart(std::string * buf)
{
	std::string::iterator	first;

	if (buf->empty())
		return NULL;
	first = buf->begin() + 1;
	while (first != buf->end() && *(first) != ':')
		first++;
	buf->erase(buf->begin(), first);
	if (buf->empty())
		return NULL;

	first = buf->begin();
	if (*first == '\n' || *first == '\r')
	{
		first = IsEOC(first, buf);
		return NULL;
	}
	return buf;
}

std::string *	Interpret::GetNextWord(std::string * buf)
{
	std::string::iterator	first(buf->begin());

	while (first != buf->end() && *first == ' ')
		first++;
	buf->erase(buf->begin(), first);
	if (buf->empty())
		return NULL;

	first = buf->begin();
	if (*first == '\n' || *first == '\r')
	{
		first = IsEOC(first, buf);
		return NULL;
	}
	return buf;
}

std::string::iterator	Interpret::IsEOC(std::string::iterator it, std::string *buf)
{
	size_t pos = it - buf->begin();

	buf->erase(it);
	it = buf->begin() + pos;
	if (*it == '\n')
	{
		buf->erase(it);
		it = buf->begin() + pos;
	}
	_iseoc = true;
	return it;
}

void	Interpret::clearLeftover(std::string * buf)
{
	std::string::iterator	it(buf->begin());

	while (it != buf->end() && *it != '\r' && *it != '\n')
		it++;
	if (it != buf->end())
	 	buf->erase(buf->begin(), ++it);
}
