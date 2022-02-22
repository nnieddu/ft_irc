/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:03 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 18:07:25 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// TEST CODE FROM CODE BISON : http://www.codingbison.com/c/c-sockets-connection-oriented.html
#include <stdio.h>
#include <errno.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define DATA_BUFFER 5000

int	start_server(int port, std::string password)
{
	server irc_server(port, password);
	
	struct sockaddr_in new_client_addr;
	int new_fd;
	int ret_val = 1;
	socklen_t addrlen = 0;
	char buf[DATA_BUFFER];

	/* Create the server socket */
	// fd = create_tcp_server_socket(port); 
	if (irc_server.getSock() == -1) 
	{
		fprintf(stderr, "Creating server failed [%s]\n", strerror(errno));
		return -1;
	}

	/* Accept a new connection */
	new_fd = accept(irc_server.getSock(), (struct sockaddr*)&new_client_addr, &addrlen);
	if (new_fd == -1) 
	{
		fprintf(stderr, "accept failed [%s]\n", strerror(errno));
		close(irc_server.getSock());
		return -1;
	}
	printf("Accepted a new connection with fd: %d\n", new_fd);

	/* Receive data */
	std::cout << "Let us wait for the client to send some data" << std::endl;
	while (ret_val > 0)
	{
		ret_val = recv(new_fd, buf, DATA_BUFFER, 0);
		if (ret_val > 0) 
			std::cout << "Received data: " << buf << std::endl;
		if (ret_val == -1) 
		{
			std::cerr << "recv() failed" << "[" << strerror(errno) << "]" << std::endl;
			break;
		}
	}
	close(new_fd);
	return(ret_val);
}

int	basic_check(int port, std::string pass)
{
	if (port <= 0 || port > 65535)
	{
		std::cout << "[Error] port need to be in 1-65535 range (prefer a port higher than 1023)." << std::endl;
		return(1);
	}
	if (pass.size() < 5) //maybe check for a max sized pass
	{
		std::cout << "[Error] you need no-empty or more robust password (more than 5 characters)" << std::endl;
		return(1);
	}
	return (0);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "[Error] need 2 args <PORT> and <PASSWORD>" << std::endl;
		return(1);
	}

	int port = atoi(av[1]);
	std::string password(av[2]);

	if (basic_check(port, password))
		return(1);
	
	return(start_server(port, password));
}