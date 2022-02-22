/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:03 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 14:49:30 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

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
	// start_test(irc_server);
	
	return(0);
}