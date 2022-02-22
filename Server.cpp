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


#include "Server.hpp"

server::server(int port, std::string password)
: _port(port), _password(password), _socket(create_tcp_server_socket(port)) {}

server::~server() { close(_socket); }

int server::getSock() const { return _socket; }

int server::create_tcp_server_socket(int port) 
{
	struct sockaddr_in saddr;
	int fd; 
	int	ret_val;

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1) 
	{
		std::cerr << "socket failed [" << strerror(errno) << "]" << std::endl;
		return -1;
	}
	
	/* Initialize the socket address structure */
	saddr.sin_family = AF_INET;         
	saddr.sin_port = htons(port); 
	saddr.sin_addr.s_addr = INADDR_ANY;
	
	/* bind the socket to port on the local host */
	ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
	if (ret_val != 0) 
	{
		std::cerr << "bind failed [" << strerror(errno) << "]" << std::endl;
		return -1;
	}
	
	/* listen for incoming connections */
	ret_val = listen(fd, 5);
	if (ret_val != 0) 
	{
		std::cerr << "listen failed [" << strerror(errno) << "]" << std::endl;
		return -1;
	}
	return fd;
}