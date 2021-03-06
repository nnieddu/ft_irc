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

#pragma once

#define RPL_WELCOME				"001"
#define RPL_YOURHOST			"002"
#define RPL_CREATED				"003"
#define RPL_MYINFO				"004"

#define RPL_UMODEIS				"221"

#define RPL_ADMINME				"256"
#define RPL_ADMINLOC1			"257"

#define RPL_AWAY				"301"
#define RPL_UNAWAY				"305"
#define RPL_NOWAWAY				"306"

#define RPL_ENDOFWHO			"315"

#define RPL_LISTSTART			"321"
#define RPL_LIST				"322"
#define RPL_LISTEND				"323"
#define RPL_CHANNELMODEIS		"324"
#define	RPL_UNIQOPIS			"325"
#define	RPL_NOTOPIC				"331"
#define RPL_TOPIC				"332"
#define RPL_INVITING			"341"
#define RPL_VERSION				"351"
#define RPL_WHOREPLY			"352"
#define RPL_NAMREPLY			"353"
#define RPL_ENDOFNAMES			"366"

#define RPL_INFO				"371"
#define RPL_ENDOFINFO			"374"
#define RPL_YOUREOPER			"381"

#define RPL_TIME    			"391"

#define ERR_NOSUCHNICK			"401"
#define ERR_NOSUCHSERVER		"402"
#define ERR_NOSUCHCHANNEL		"403"
#define ERR_CANNOTSENDTOCHAN	"404"
#define ERR_TOOMANYCHANNELS		"405"
#define ERR_TOOMANYTARGETS		"407"
#define ERR_NOORIGIN			"409"
#define ERR_NORECIPIENT			"411"
#define ERR_NOTEXTTOSEND		"412"
#define ERR_NOTOPLEVEL			"413"
#define ERR_WILDTOPLEVEL		"414"

#define ERR_INPUTTOOLONG		"417"

#define ERR_UNKNOWNCOMMAND		"423"
#define ERR_NOADMININFO 		"423"

#define ERR_ERRONEUSNICKNAME	"432"

#define ERR_NONICKNAMEGIVEN		"431"
#define ERR_ERRONEUSNICKNAME	"432"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_USERNOTINCHANNEL	"441"
#define ERR_NOTONCHANNEL		"442"
#define ERR_USERONCHANNEL		"443"
#define ERR_NEEDMOREPARAMS		"461"
#define ERR_ALREADYREGISTRED	"462"
#define ERR_PASSWDMISMATCH		"464"
#define ERR_KEYSET				"467"

#define ERR_CHANNELISFULL		"471"
#define ERR_UNKNOWNMODE		    "472"
#define ERR_INVITEONLYCHAN		"473"
#define ERR_BANNEDFROMCHAN		"474"
#define ERR_BADCHANNELKEY		"475"

#define ERR_BADCHANMASK			"476"
#define	ERR_NOCHANMODES			"477"
#define ERR_NOPRIVILEGES		"481"
#define ERR_CHANOPRIVSNEEDED	"482"
#define ERR_RESTRICTED			"484"
#define ERR_NOOPERHOST			"491"
#define ERR_USERSDONTMATCH		"502"
