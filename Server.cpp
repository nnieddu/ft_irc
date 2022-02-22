/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:26 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 14:42:27 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

server::server(int port, std::string password)
{
    std::cout << "Welcome on the server : ports = " << port << " | password = " << password << std::endl;
}

server::~server()
{
    std::cout << "destruction" << std::endl;
}
