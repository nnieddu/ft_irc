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
https://www.youtube.com/watch?v=cNdlrbZSkyQ

https://www.irchelp.org/protocol/rfc/

https://www.alien.net.au/irc/  
https://fr.wikipedia.org/wiki/Aide:IRC/commandes

### Testing :  
Weechat irc client for testing :  
apt-get install weechat-curses weechat-plugins 

netcat testing :
```
nc 127.0.01 <PORT>
```
### Docs :
#### RFC 1459 :
##### Clients
A client is anything connecting to a server that is not another server.   
Each client is distinguished from other clients by a unique nickname having a maximum length of nine (9) characters.  

##### Operators
A special class of clients (operators) who's allowed to perform general maintenance functions on the network.  
  
##### Server
all servers must have the following information about all clients: 
- the real name of the host that the client is running on, 
- the username of the client on that host, 
- and the server to which the client is connected.
   
##### Channels 
A channel is a named group of one or more clients which will all receive messages addressed to that channel.  
The channel is created implicitly when the first client joins it, and the channel ceases to exist when the last client leaves it.  
While channel exists, any client can reference the channel using the name of the channel.  
  
Channels names are strings (beginning with a '&' or '#' character) of length up to 200 characters. Apart from the the requirement that the first character being either '&' or '#'; the only restriction on a channel name is that it may not contain any spaces (' '), a control G (^G or ASCII 7), or a comma (',' which is used as a list item separator by the protocol).
###### Channel Operators
The channel operator (also referred to as a "chop" or "chanop") on a given channel is considered to 'own' that channel.
A channel operator is identified by the '@' symbol next to their nickname whenever it is associated with a channel (ie replies to the NAMES, WHO and WHOIS commands).
The commands which may only be used by channel operators are:
- KICK    - Eject a client from the channel
- MODE    - Change the channel's mode
- INVITE  - Invite a client to an invite-only channel (mode +i)
- TOPIC   - Change the channel topic in a mode +t channel
   
    
### implemented commands : ( [4. Message details](https://www.rfcreader.com/#rfc1459_line568) )
| command | params | description |
|---|---|---|
| PASS | `<password>` | used to set up a 'password connection'. a password must be set before any attempt. |
| NICK | `<nickname>` | used to assign an nickname to the user or change the previous one. there can be no two identical nicknames on the server. |
| USER | `<username>` `<hostname>` `<servername>` `<realname>` | used at the beginning of a connection to specify the username, hostname, server name and real name of the new user. |
| OPER | `<user>` `<password>` | used to take operator rights. parameters are used to identify the user. |
| QUIT | `[<quit message>]` | used to terminate the connection to the server. the server should end the connection with the client when it sees the sent message. |
| PING / PONG | `<server1>` `[<server2>]` | used to check for client activity on the other end of the connection. this message is sent at regular intervals unless other activity is noticed from the connection. if the client does not send PONG in response to PING, the connection is closed. |
| PRIVMSG | `<receiver>{,<receiver>}` `<text to be sent>` | used for private correspondence between users. also exists the ability to send messages to channels. |
| NOTICE | `<nickname>` `<text>` | works the same as PRIVMSG, except that no response is sent in response to a message. |
| JOIN | `<channel>{,<channel>}` `[<key>{,<key>}]` | used by the client to enter the channel. if a password is set, it must be correct. when users enter a channel, they will receive a notification about all users on the channel. if there was no group before, then the group is created. |
| PART | `<channel>{,<channel>}` | used by the user to leave the channels that he will specify in the parameters. |
| LIST | `[<channel>{,<channel>}` `[<server>]]` | used to display a list of channels and their topics. if the channel parameter is used, then only the status of this channel is displayed. |
| INVITE | `<nickname>` `<channel>` | used to invite users to the channel. if the channel is open, then only the channel operator can invite to it. |
| TOPIC | `<channel>` `[<topic>]` | used to edit or view a channel topic. the channel topic will remain the same unless a new topic is set. |
| NAMES | `[<channel>{,<channel>}]` | used by a user can get a list of all users in the channel. |
| KICK | `<channel>` `<user>` `[<comment>]` | used to exclude a user from a channel can only be used by the channel operator. |
| KILL | `<user>` `<message>` | disconnects the user from the server. |

      
### Useful infos :
##### Ports :
1-65535 are available, and ports in range 1-1023 are the privileged ones: an application needs to be run as root in order to listen to these ports.
