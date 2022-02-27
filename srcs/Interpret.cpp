
#include "../incs/Interpret.hpp"
#include "../incs/Socket.hpp"
#include "../incs/NumericReplies.hpp"
#include "../incs/Server.hpp"
#include "../incs/Commands.hpp"

#include <stdlib.h>

Interpret::Interpret(server * serv): _serv(serv)
{
	cmds_list["PASS"] = new Pass(_serv);
	cmds_list["NICK"] = new Nick(_serv);
	cmds_list["USER"] = new User(_serv);
	// cmds_list["OPER"] = Oper(_serv);
	// cmds_list["QUIT"] = Quit(_serv);
	cmds_list["JOIN"] = new Join(_serv);
	// cmds_list["PART"] = Part(_serv);
	// cmds_list["MODE"] = Mode(_serv);
	// cmds_list["TOPIC"] = Topic(_serv);
	// cmds_list["NAMES"] = Names(_serv);
	cmds_list["LIST"] = new List(_serv);
	// cmds_list["INVITE"] = Invite(_serv);
	// cmds_list["KICK"] = Kick(_serv);
	// cmds_list["VERSION"] = Version(_serv);
	// cmds_list["STATS"] = Stat(_serv);
	// cmds_list["TIME"] = Time(_serv);
	// cmds_list["ADMIN"] = Admin(_serv); // maybe useless
	// cmds_list["INFO"] = Info(_serv);
	// cmds_list["PRIVMSG"] = Privmsg(_serv);
	// cmds_list["NOTICE"] = Notice(_serv);
	// cmds_list["WHO"] = Who(_serv);
	// cmds_list["WHOIS"] = Whois(_serv);
	// cmds_list["WHOWAS"] = Whowas(_serv); // a voir mais relou
	// cmds_list["KILL"] = Kill(_serv);
	// cmds_list["PING"] = Ping(_serv);
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
	std::vector<std::string>					args;
	std::map<std::string, Command *>::iterator	it;

	int ret = 0;
	
	while ((usr.buf.find('\r') && ret == 0))
	{
		if ((it = cmds_list.find(parseCmds(&usr.buf))) != cmds_list.end())
		{
			cmd = it->second;
			cmd->setExpeditor(&usr);

			if (cmd->needChannel())
				args.push_back(parseChannel(&usr.buf));
			if (cmd->needUser())
				args.push_back(parseNick(&usr.buf));
			if (cmd->needArg())
				args.push_back(parseArg(&usr.buf));
			cmd->setArgs(args);
			ret = cmd->execute();
			cmd->reset();
		}
		else
			break;
	}
	// std::cout << "Command not found" << std::endl;
	 // send replies cmd not found ou envoyer msg dans chan
	return ret;
}

std::string Interpret::parseCmds(std::string * buf)
{
	std::string::iterator	it = buf->begin();
	std::string				cmd;

	while (*it != ' ' && it != buf->end())
		it++;
	cmd.assign(buf->begin(), it);
	buf->erase(buf->begin(), it);
	return (cmd);
}

std::string	Interpret::parseChannel(std::string * buf)	//parseChannel et parseNick identique atm, les channels doivent commencer par '#' donc veridier si on peut improve
{
	std::string::iterator	first(buf->begin());
	std::string::iterator	last;
	std::string				arg;

	while (*first == ' ')
		first++;
	buf->erase(buf->begin(), first - 1);
	if (*first == '\r' || *first == '\n')
	{
		buf->erase(buf->find("\r"));
		return "";
	}
	if (first == buf->end())
		return "";
	last = first;
	while(last != buf->end() && *last != ' ' && *last != '\r' && *last != '\n' )
		last++;
	if (last != buf->end() && *last == '\r' || *last == '\n')
		buf->erase(buf->find("\r"));
	arg.assign(first, last);
	buf->erase(first, last);
	return arg;
}

std::string	Interpret::parseNick(std::string * buf)
{
	std::string::iterator	first(buf->begin());
	std::string::iterator	last;
	std::string				arg;

	while (*first == ' ')
		first++;
	buf->erase(buf->begin(), first - 1);
	if (*first == '\r' || *first == '\n')
	{
		buf->erase(buf->find("\r"));
		return "";
	}
	if (first == buf->end())
		return "";
	last = first;
	while(last != buf->end() && *last != ' ' && *last != '\r' && *last != '\n' )
		last++;
	if (last != buf->end() && *last == '\r' || *last == '\n')
		buf->erase(buf->find("\r"));
	arg.assign(first, last);
	buf->erase(first, last);
	return arg;
}

std::string	Interpret::parseArg(std::string * buf)
{
	std::string::iterator	first(buf->begin());
	std::string::iterator	last;
	std::string				arg;

	while (*first == ' ')
		first++;
	buf->erase(buf->begin(), first - 1);
	if (first == buf->end())
		return "";
		// return "";
	// if (*first == '\r' || *first == '\n')
	// {
	// 	buf->erase(buf->find("\r"));
	// 	return "";
	// }
	// std::cout << "\n---- [ARGS] ---\n";
	last = first;
	while(last != buf->end() && *last != '\r' && *last != '\n' )
		last++;
	// if (last != buf->end() && *last == '\r' || *last == '\n') // 
	// 	buf->erase(buf->find("\r"));
	arg.assign(first, last);
	buf->erase(first, last);
	return arg;
}
