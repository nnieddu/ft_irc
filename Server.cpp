/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:26 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 23:56:42 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <errno.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include "Server.hpp"
#include "User.hpp"

server::server(const int & port, const std::string & password):_port(port), _password(password), _socket(port)
{
	if (_port <= 0 || _port > 65535)
		throw(std::invalid_argument(std::string("port number")));
	if (_password.size() < 5)
		throw(std::invalid_argument(std::string("password")));
}


server::~server() {}

int server::getSock() const { return _socket.fd; }

int		server::start()
{
	int		ret_val(1);
	char	buf[DATA_BUFFER];

	_socket.init();

	std::cout << "Welcome on the IRC server !" << std::endl
		<< "Waiting for connection..." << std::endl;

	user	new_user("tmp", "tmp", false, getSock());

	std::cout << "Accepted a new connection with fd: " << new_user.getSock() << std::endl;


	std::string response = "Good talking to you\n";
 	send(new_user.getSock(), response.c_str(), response.size(), 0);
	
	std::cout << "Let us wait for the client to send some data" << std::endl;
	while (ret_val != 0)
	{
		ret_val = recv(new_user.getSock(), buf, DATA_BUFFER, 0);
		std::cout << "Received data (len " << ret_val << " bytes)" << std::endl;
		if (ret_val > 0) 
			std::cout << "Received data: " << std::string(buf) << std::endl;
		if (ret_val == -1) 
			throw(std::runtime_error("recv"));
	}

	return(ret_val);
}
