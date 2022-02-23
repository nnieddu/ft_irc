/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 22:48:07 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 23:42:35 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include "Socket.hpp"

class user
{
	private:

		user();
		user(const user& x);
		user& operator=(const user& x);
		
		std::string	_name;
		std::string	_password;
		Socket		_socket;
		bool		_isOperator;

	public:

		user(std::string name, std::string password, bool isOperator, int serverSocket);
		virtual ~user();
	
		int						getSock() const;
		struct sockaddr_in		getsaddr() const;
		socklen_t				getaddrlen() const;
};