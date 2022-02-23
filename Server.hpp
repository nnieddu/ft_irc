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

#include <stdio.h>
#include <errno.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

class server
{
	private:
		server();
		server(const server& x);
		server& operator=(const server& x);
		
		int			_port;
		std::string	_password;
		int			_socket;
		int create_tcp_server_socket(int port);
	public:
		server(int port, std::string password);
		~server();

		std::map<std::string, int>	_users;
		std::map<std::string, int>	_channels;
		
		int	getSock()const;
};
