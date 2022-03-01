
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
	cmds_list["PRIVMSG"] = new Privmsg(_serv);
	// cmds_list["NOTICE"] = Notice(_serv);
	// cmds_list["WHO"] = Who(_serv);
	// cmds_list["WHOIS"] = Whois(_serv);
	// cmds_list["WHOWAS"] = Whowas(_serv); // a voir mais relou
	// cmds_list["KILL"] = Kill(_serv);
	cmds_list["PING"] = new Ping(_serv);
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
		if ((it = cmds_list.find(parseCmds(&usr.buf))) != cmds_list.end())
		{
			cmd = it->second;
			cmd->setExpeditor(&usr);

			if (cmd->needNick())
				args.push_back(parseWord(&usr.buf));
			if (cmd->needChannel())
				args.push_back(parseWord(&usr.buf));
			if (cmd->needUser())
				args.push_back(parseWord(&usr.buf));
			if (cmd->needArg())
				args.push_back(parseAll(&usr.buf));

			cmd->setArgs(args);
			ret = cmd->execute();
			cmd->reset();
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

	while (it != buf->end() && *it != '\r' && *it != '\n' && *it != ' ')
		it++;
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

	// std::cerr << "Command not found" << std::endl; // pass tt le temps par la 
	return 1;
}

std::string *	Interpret::parseWord(std::string * buf)	//parseChannel et parseNick identique atm, les channels doivent commencer par '#' donc veridier si on peut improve
{
	std::string::iterator	first(buf->begin());
	std::string::iterator	last;
	std::string				arg;

	if (GetNextWord(buf) == NULL)
		return NULL;
	first = buf->begin();
	last = first;
	while(last != buf->end() && *last != ' ' && *last != '\r' && *last != '\n' )
		last++;
	if (last != buf->end() && *last == '\r' || *last == '\n')
		buf->erase(last);
	arg.assign(first, last);
	buf->erase(first, last);
	return new std::string(arg);
}

std::string *	Interpret::parseAll(std::string * buf)
{
	std::string::iterator	first;
	std::string::iterator	last;
	std::string				arg;

	if (GetNextWord(buf) == NULL)
		return NULL;
	first = buf->begin();
	last = first;
	while(last != buf->end() && *last != '\r' && *last != '\n' )
		last++;
	if (last != buf->end())
	 	buf->erase(last);
	arg.assign(first, last);
	buf->erase(first, last);
	return new std::string(arg);
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
		buf->erase(first);
		return NULL;
	}
	return buf;
}

// Les deux bout commented font segfault si "PASS " sur netcat
// si un champs vide pour le pass est donne dans weechat, il nenvoit pas la commande PASS