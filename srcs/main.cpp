/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:03 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/28 16:02:12 by mrozniec         ###   ########.fr       */
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
		throw(server::quitexcept());
		server	serv(atoi(av[1]), std::string(av[2]));
		return (serv.run());
	}
	catch (server::quitexcept er)
	{
		std::cout << er.what() << std::endl;
		ret = 0;
	}
	catch (std::runtime_error &er)
	{
		std::cerr << er.what() << ": " << std::strerror(errno) << std::endl;
	}
	catch (std::invalid_argument &er)
	{
		std::cerr << "[error]: invalid argument : " << er.what() << std::endl;
	}
	catch (std::exception &er)
	{
		std::cerr << er.what() << std::endl;
	}
	return(ret);
}
