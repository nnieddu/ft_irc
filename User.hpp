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
#include <map>

#include <stdio.h>
#include <errno.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

class user
{
	private:
		user();
		user(const user& x);
		user& operator=(const user& x);
		
		std::string	_name;
		std::string	_password;
		int			_socket;
		bool		_isOperator;
	public:
		user(std::string name, std::string password, bool isOperator, int serverSocket);
		~user();
		
		int	getSock()const;
};