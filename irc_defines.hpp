/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_defines.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 00:05:38 by ninieddu          #+#    #+#             */
/*   Updated: 2022/02/23 00:14:29 by ninieddu         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// https://www.alien.net.au/irc/irc2numerics.html

#pragma once

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"
#define RPL_BOUNCE "005"

#define RPL_TRACELINK "200"
#define RPL_TRACECONNECTING "201"
#define RPL_TRACEHANDSHAKE "202"
#define RPL_TRACEUNKNOWN "203"
#define RPL_TRACEOPERATOR "204"
#define RPL_TRACEUSER "205"
#define RPL_TRACESERVER "206"

#define RPL_TRACESERVICE "207"
#define RPL_TRACENEWTYPE "208"
#define RPL_TRACECLASS "209"
#define RPL_TRACERECONNECT "210"