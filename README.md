# ft_irc
L’objectif de ce projet est de reproduire le fonctionnement d’un serveur IRC. 

Internet Relay Chat est un protocole de communication textuel sur Internet.
Il sert à la communication instantanée principalement sous la forme de discussions en groupe par l’intermédiaire
de canaux de discussion, mais peut aussi être utilisé pour de la communication directe entre deux personnes.
Les clients IRC se connectent à des serveurs IRC afin d’accéder à des canaux. Les serveurs IRC sont connectés 
entre eux afin de créer des réseaux.

  
[RFC 1459 Internet Relay Chat: (updated by belows)](https://www.rfcreader.com/#rfc1459)  
[RFC 2810 Internet Relay Chat: Architecture](https://www.rfcreader.com/#rfc2810)  
[RFC 2811 Internet Relay Chat: Channel Management](https://www.rfcreader.com/#rfc2811)  
[RFC 2812 Internet Relay Chat: Client Protocol](https://www.rfcreader.com/#rfc2812)  
[RFC 2813 Internet Relay Chat: Server Protocol](https://www.rfcreader.com/#rfc2813)  
	
https://www.tutorialspoint.com/websockets/index.htm  
http://vidalc.chez.com/lf/socket.html  
https://ncona.com/2019/04/building-a-simple-server-with-cpp/  
https://vichargrave.github.io/programming/tcp-ip-network-programming-design-patterns-in-cpp/   
https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select
https://www.irchelp.org/protocol/rfc/

https://www.alien.net.au/irc/  
https://fr.wikipedia.org/wiki/Aide:IRC/commandes

### Testing :  
[Weechat irc client for testing :](https://weechat.org/files/doc/stable/weechat_quickstart.fr.html#start)
```
apt-get install weechat-curses weechat-plugins 
```
```
/server add <NAME> 127.0.0.1/<PORT>
and
/connect <NAME> -password=<PASWORD>
```

##### netcat testing :
```
nc 127.0.01 <PORT>
```

```
PASS password
NICK test
USER test 0 * :testing name
```

### Docs / Useful infos:

##### Clients
A client is anything connecting to a server that is not another server.   
Each client is distinguished from other clients by a unique nickname having a maximum length of nine (9) characters.  

##### Operators
A special class of clients (operators) who's allowed to perform general maintenance functions on the network.  
   
##### Channels 
A channel is a named group of one or more clients which will all receive messages addressed to that channel.  
The channel is created implicitly when the first client joins it, and the channel ceases to exist when the last client leaves it.  
While channel exists, any client can reference the channel using the name of the channel.  
  
Channels names are strings (beginning with a '&' or '#' character) of length up to 200 characters. Apart from the the requirement that the first character being either '&' or '#'; the only restriction on a channel name is that it may not contain any spaces (' '), a control G (^G or ASCII 7), or a comma (',' which is used as a list item separator by the protocol).

##### Channel Operators
The channel operator (also referred to as a "chop" or "chanop") on a given channel is considered to 'own' that channel.
A channel operator is identified by the '@' symbol next to their nickname whenever it is associated with a channel (ie replies to the NAMES, WHO and WHOIS commands).
The commands which may only be used by channel operators are:
- KICK    - Eject a client from the channel
- MODE    - Change the channel's mode
- INVITE  - Invite a client to an invite-only channel (mode +i)
- TOPIC   - Change the channel topic in a mode +t channel
   
	
#### RFC commands :
| command | params | description |
|---|---|---|
| PASS | `<password>` | used to set up a 'password connection'. a password must be set before any attempt. |
| NICK | `<nickname>` | used to assign an nickname to the user or change the previous one. there can be no two identical nicknames on the server. |
| USER | `<username>` `<hostname>` `<servername>` `<realname>` | used at the beginning of a connection to specify the username, hostname, server name and real name of the new user. |
| OPER | `<user>` `<password>` | used to take operator rights. parameters are used to identify the user. |
| QUIT | `[<quit message>]` | used to terminate the connection to the server. the server should end the connection with the client when it sees the sent message. |
| JOIN | `<channel>{,<channel>}` `[<key>{,<key>}]` | used by the client to enter the channel. if a password is set, it must be correct. when users enter a channel, they will receive a notification about all users on the channel. if there was no group before, then the group is created. |
| PART | `<channel>{,<channel>}`, `<message>`]| used by the user to leave the channels that he will specify in the parameters. |
| MODE |`<channel>`  or `<user>` | Set the mode for the user or channel (see Mode below) |
| TOPIC | `<channel>` `[<topic>]` | used to edit or view a channel topic. the channel topic will remain the same unless a new topic is set. |
| NAMES | `[<channel>{,<channel>}]` | used by a user can get a list of all users in the channel. |
| LIST | `[<channel>{,<channel>}` `[<server>]]` | used to display a list of channels and their topics. if the channel parameter is used, then only the status of this channel is displayed. |
| INVITE | `<nickname>` `<channel>` | used to invite users to the channel. if the channel is open, then only the channel operator can invite to it. |
| KICK | `<channel>` `<user>` `[<comment>]` | used to exclude a user from a channel can only be used by the channel operator. |
| VERSION | | Display information on the server version |
| TIME | `<server>`  | The time message is used to query local time from the specified server.|
| INFO |  | Display informations of the server |
| PRIVMSG | `<receiver>{,<receiver>}` `<text to be sent>` | used for private correspondence between users. also exists the ability to send messages to channels. |
| NOTICE | `<nickname>` `<text>` | works the same as PRIVMSG, except that no response is sent in response to a message. |
| WHO | `<name> [<o>]`| list users and her informations if no param is given |
| KILL | `<user>` `<comment>` | disconnects the user from the server. |
| DIE | | Shutdown the server (need irc operator right) |
| AWAY | | Set user mode 'away' with a message. |
| PING / PONG | `<server1>` `[<server2>]` | used to check for client activity on the other end of the connection. this message is sent at regular intervals unless other activity is noticed from the connection. if the client does not send PONG in response to PING, the connection is closed. |

##### Ports :
1-65535 are available, ports in range 1-1023 are the privileged ones: an application needs to be run as root in order to listen to these ports.

#### Modes : 
###### User modes
Parameters: <nickname> [+|-]|[flag]

The user MODEs are typically changes which affect either how the
client is seen by others or what 'extra' messages the client is sent.
A user MODE command may only be accepted if both the sender of the
message and the nickname given as a parameter are both the same.

The available modes are as follows:
	
	a - marks a users as away;
	i - marks a users as invisible;
	o - operator flag.
	w - user receives wallops;
	r - restricted mode (can't change nickname and nor make use of the channel operator status)

###### Channel modes
Parameters: <channel> [+|-]|[flag] [param]
The various modes available for channels are as follows:

	a - made a channel and is users Anonymous;
	i - invite-only channel flag;
	k - set a channel key (password).
	l - set the user limit to channel;
	m - moderated channel;
	n - no messages to channel from clients on the outside;
	o - give/take channel operator privileges;
	p - private channel flag;
	t - topic settable by channel operator only flag;
	r - choose random channel after a certain amount of time if the last one left.
	v - give/take the ability to speak on a moderated ('m') channel;
