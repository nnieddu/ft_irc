/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrozniec <mrozniec@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:15:52 by mrozniec          #+#    #+#             */
/*   Updated: 2022/03/07 17:11:24 by mrozniec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <set>
#include "User.hpp"
#include <exception>
#include <ctime>

//MOD:
//member status
#define O 0x00001	// *give channel creator status
#define o 0x00002	// *give/take channel operator status || user is server op
#define v 0x00004	// give/take voice privilege || userflag on a specific chan
//channel flag
#define a 0x00008	// anonymous channel flag   || user flag as away
#define i 0x00010	// invite-only channel flag || invisible user flag
#define m 0x00020	// moderated channel flag
#define n 0x00040	// no messages to channel from clients on the outside
#define q 0x00080	// quiet channel flag
#define p 0x00100	// private channel flag
#define r 0x00400	// server reop channel flag || restricted user connection user flag
#define t 0x00800	// topic settable only by op

#define k 0x01000	// set/remove channel key(password)
#define l 0x02000	// *set/remove user limit to channel

#define RFLAG_TIME_SEC	5	// time in sec before a channel with -r flag will chose a random new operator amongst its users


class user;

class Channel {
	private:
		Channel();

		unsigned int		mod;
		unsigned int		limit_user;
		std::string			name;
		std::string			id;
		std::string			topic;
		std::string			password;
		user*				chanCrea;
		std::set<user *>	users;
		std::set<user *>	users_invited;
		bool				hasop;
		std::time_t			rtime;

	public:
		Channel(user&, std::string&);
		Channel& operator=(const Channel&);
		virtual	~Channel();

		void	setMod(unsigned int);
		void	removeMod(unsigned int);
		void	setLim(unsigned int);
		void	setTopic(const std::string&);
		void	setPass(const std::string&);
		void	addUser(user&);
		void	removeUser(user&);

		const unsigned int&		getMod() const;
		const unsigned int&		getLim() const;
		const std::string&		getName() const;
		const std::string&		getId() const;
		const std::string&		getTopic() const;
		const std::string&		getPass() const;
		const user&				getChanCrea() const;
		const std::set<user*>&	getUsers() const;

		bool					isInvited(user&) const;
		void					addInvited(user&);
		void					remInvited(user&);

		void 	send_names_replies(const user*) const;

		bool	mustAddOp(const std::time_t &) const;
		void	rdmOp(const std::time_t &);

		bool	geta() const;
		bool	geti() const;
		bool	getm() const;
		bool	getn() const;
		bool	getq() const;
		bool	getp() const;
		bool	getr() const;
		bool	gett() const;
		bool	getk() const;
		bool	getl() const;
};
