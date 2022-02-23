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
#include "Socket.hpp"

user::user(std::string name, std::string password, bool isOperator, int serverSocket)
: _name(name), _password(password), _isOperator(isOperator) 
{
	_socket.len = 0;
	_socket.fd = accept(serverSocket, reinterpret_cast<sockaddr*>(&_socket.address), &_socket.len);
	if (_socket.fd == -1)
		throw(std::runtime_error("accept"));
}

user::~user() {}

int user::getSock() const
{
	return _socket.fd;
}
