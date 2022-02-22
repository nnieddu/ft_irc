/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:42:28 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/22 14:43:42 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <map>

class server
{
	//coplien/canonique form 
	private:
		server();
		server(const server&);
		server& operator=(const server& x);

		std::map<int, std::string> _users;
	public:
		server(int port, std::string password);
		~server();
};
