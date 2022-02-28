/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:03 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/23 21:29:27 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"

int main(int ac, char **av)
{
	int	ret = 1;

	if (ac != 3)
	{
		std::cerr << "[Error] : need 2 args <PORT> and <PASSWORD>" << std::endl;
		return(ret);
	}
	try
	{
		server	serv(atoi(av[1]), std::string(av[2]));
		return (serv.run());
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << ": " << std::strerror(errno) << std::endl;
	}
	catch (std::invalid_argument &e)
	{
		std::cerr << "[error]: invalid argument : " << e.what() << std::endl;
	}
	catch (server::quitexcept &e)
	{
		std::cout << e.what() << std::endl;
		ret = 0;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return(ret);
}
