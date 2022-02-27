/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrozniec <mrozniec@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:15:52 by mrozniec          #+#    #+#             */
/*   Updated: 2022/02/27 22:53:55 by mrozniec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "User.hpp"
#include <exception>

//MOD:
//member status
#define O 0x00001	// give channel creator status
#define o 0x00002	// give/take channel operator status
#define v 0x00004	// give/take voice privilege
//channel flag
#define a 0x00008	// anonymous channel flag
#define i 0x00010	// invite-only channel flag
#define m 0x00020	// moderated channel flag
#define n 0x00040	// no messages to channel from clients on the outside
#define q 0x00080	// quiet channel flag
#define p 0x00100	// private channel flag
#define s 0x00200	// secret channel flag
#define r 0x00400	// server reop channel flag
#define t 0x00800	// topic settable only by op

#define k 0x01000	// set/remove channel key(password)
#define l 0x02000	// set/remove user limit to channel
//channel access control TODO:: need to implement it later(using IP ??)
#define b 0x04000	// set/remove ban mask to keep user out
#define e 0x08000	// set/remove except mask to override ban mask
#define I 0x10000	// set/remove an invit mask to automatically override
					//the invite only flag

class Channel {
	private:
		unsigned int		mod;
		unsigned int		limit_user;
		std::string			name;
		std::string			id;
		std::string			topic;
		std::string			password;
		user*				chanCrea;
		std::vector<user *>	chanOps;
		std::vector<user *>	users;

	public:
		Channel(user&, std::string&);

		void	setMod(unsigned int);
		void	setLim(unsigned int);
		void	setTopic(const std::string&);
		void	setPass(const std::string&);
		void	addOp(user&);
		void	addUser(user&);

		const unsigned int&			getMod() const;
		const unsigned int&			getLim() const;
		const std::string&			getName() const;
		const std::string&			getId() const;
		const std::string&			getTopic() const;
		const std::string&			getPass() const;
		const user&					getChanCrea() const;
		const std::vector<user*>&	getChanOps() const;
		const std::vector<user*>&	getUsers() const;

};
