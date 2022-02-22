/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:03 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 22:35:51 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	start_server(int port, std::string password)
{
	server irc_server(port, password);
		
	struct sockaddr_in new_client_addr;
	int new_fd;
	int ret_val = 1;
	socklen_t addrlen = 0;
	char buf[5000];

	if (irc_server.getSock() == -1) 
	{
		std::cerr << "Creating server failed  [" << strerror(errno) << "]" << std::endl;
		return -1;
	}
	
	/* Accept a new connection */
	new_fd = accept(irc_server.getSock(), (struct sockaddr*)&new_client_addr, &addrlen);
	if (new_fd == -1) 
	{
		std::cerr << "accept failed [" << strerror(errno) << "]" << std::endl;
		return -1;
	}
	
	std::cout << "Accepted a new connection with fd : " << new_fd << std::endl;

	/* Receive data */
	std::cout << "Let us wait for the client to send some data" << std::endl;
	while (ret_val > 0)
	{
		ret_val = recv(new_fd, buf, sizeof(buf), 0);
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

int	input_check(int port, std::string pass)
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

	if (input_check(port, password))
		return(1);
	
	return(start_server(port, password));
}