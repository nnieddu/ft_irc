/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:28 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/23 10:57:49 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>
#include "Socket.hpp"
#include "User.hpp"
//#include "Channel.hpp"

#define DATA_BUFFER 50000

class server
{
	private:

		server();
		server(const server& x);
		server& operator=(const server& x);
		
		int					_port;
		std::string			_password;
		Socket				_socket;

		std::map<std::string, user>	_users;
//		std::map<int, channel>	_channels;

	public:

		server(const int & port, const std::string & password);
		~server();
		
		int	getSock() const;

		int	start();
};
