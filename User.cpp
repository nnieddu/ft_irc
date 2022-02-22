/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:26 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 23:44:22 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

user::user(std::string name, std::string password, bool isOperator, int serverSocket)
: _name(name), _password(password), _isOperator(isOperator) 
{
	struct sockaddr_in  new_client_addr;
	socklen_t           addrlen = 0;

	_socket = accept(serverSocket, (struct sockaddr*)&new_client_addr, &addrlen);

}

user::~user() { close(_socket); }

int user::getSock() const { return _socket; }
