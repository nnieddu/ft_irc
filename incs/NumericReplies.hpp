/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 00:05:38 by ninieddu          #+#    #+#             */
/*   Updated: 2022/03/02 19:50:26 by mrozniec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://www.alien.net.au/irc/irc2numerics.html

// PART 4 a Part 6
// https://www.rfcreader.com/#rfc1459_line568 -> https://www.rfcreader.com/#rfc1459_line2633

#pragma once

#define RPL_WELCOME "001"
// #define RPL_YOURHOST "002"
// #define RPL_CREATED "003"
// #define RPL_MYINFO "004"
// #define RPL_BOUNCE "005"
// 001 	RPL_WELCOME 	RFC2812 	:Welcome to the Internet Relay Network <nick>!<user>@<host> 	
// 002 	RPL_YOURHOST 	RFC2812 	:Your host is <servername>, running version <version> 	Part of the post-registration greeting. Text varies widely
// 003 	RPL_CREATED 	RFC2812 	:This server was created <date> 	Part of the post-registration greeting. Text varies widely
// 004 	RPL_MYINFO 	RFC2812 	<server_name> <version> <user_modes> <chan_modes> 	Part of the post-registration greeting 


#define RPL_AWAY "301"
#define RPL_LISTSTART "321"
#define RPL_LIST "322"
#define RPL_LISTEND "323"
#define RPL_TOPIC "332"
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"
// 353 	RPL_NAMREPLY 	RFC1459 	
// ( '=' / '*' / '@' ) <channel> ' ' : [ '@' / '+' ] <nick> *( ' ' [ '@' / '+' ] <nick> ) 	Reply to NAMES (See RFC) 

#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHSERVER "402"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYTARGETS "407"
#define ERR_NOORIGIN "409"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_NOTOPLEVEL "413"
#define ERR_WILDTOPLEVEL "414"


#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NOTONCHANNEL "442"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_PASSWDMISMATCH "464"

#define ERR_BADCHANMASK "476"
