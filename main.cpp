/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:03 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 16:47:14 by ninieddu         ###   ########lyon.fr   */
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

int create_tcp_server_socket(int port) 
{
	struct sockaddr_in saddr;
	int fd, ret_val;

	/* Step1: create a TCP socket */
	// int socket(int domain, int type, int protocol);
	// AF_INET = IPv4 Internet protocols, SOCK_STREAM = Provides sequenced, reliable, two-way, connection-based byte streams.
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1) 
	{
		fprintf(stderr, "socket failed [%s]\n", strerror(errno));
		close(fd);
		return -1;
	}
	printf("Created a socket with fd: %d\n", fd);

	/* Initialize the socket address structure */
	saddr.sin_family = AF_INET;         
	saddr.sin_port = htons(port); //convert values between host and network byte order 
	saddr.sin_addr.s_addr = INADDR_ANY; // Address to accept any incoming messages.

	/* Step2: bind the socket to port on the local host */
	ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
	if (ret_val != 0) 
	{
		fprintf(stderr, "bind failed [%s]\n", strerror(errno));
		close(fd);
		return -1;
	}

	/* Step3: listen for incoming connections */
	ret_val = listen(fd, 5);
	if (ret_val != 0) 
	{
		fprintf(stderr, "listen failed [%s]\n", strerror(errno));
		close(fd);
		return -1;
	}
	return fd;
}

int	start_test(int port)
{
	struct sockaddr_in new_client_addr;
	int fd, new_fd;
	int ret_val = 1;
	socklen_t addrlen;
	char buf[DATA_BUFFER];

	/* Create the server socket */
	fd = create_tcp_server_socket(port); 
	if (fd == -1) 
	{
		fprintf(stderr, "Creating server failed [%s]\n", strerror(errno));
		return -1;
	}

	/* Accept a new connection */
	new_fd = accept(fd, (struct sockaddr*)&new_client_addr, &addrlen);
	if (new_fd == -1) 
	{
		fprintf(stderr, "accept failed [%s]\n", strerror(errno));
		close(fd);
		return -1;
	}
	printf("Accepted a new connection with fd: %d\n", new_fd);

	/* Receive data */
	std::cout << "Let us wait for the client to send some data" << std::endl;
	while (ret_val > 0)
	{
		ret_val = recv(new_fd, buf, DATA_BUFFER, 0);
		printf("Received data (len %d bytes)\n", ret_val);
		if (ret_val > 0) 
			std::cout << "Received data: " << buf << std::endl;
		if (ret_val == -1) 
		{
			std::cerr << "recv() failed" << "[" << strerror(errno) << "]" << std::endl;
			break;
		}
	}
	/* Close the sockets */
	close(fd);
	close(new_fd);
	return(ret_val);
}
//----------------------------------------------------end of code bison part


int	basic_check(int port, std::string pass)
{
	if (port <= 0 || port > 65535)
	{
		std::cout << "[Error] : port need to be in 1-65535 range (prefer a port higher than 1023)." << std::endl;
		return(1);
	}
	if (pass.size() < 5) //maybe check for a max sized pass
	{
		std::cout << "[Error] : you need no empty password or more robust (more than 5 characters)" << std::endl;
		return(1);
	}
	return (0);
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "[Error] : need 2 args <PORT> and <PASSWORD>" << std::endl;
		return(1);
	}

	int port = atoi(av[1]);
	std::string password(av[2]);

	if (basic_check(port, password))
		return(1);

	server irc_server(port, password);
	
	return(start_test(port));
}