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
/server add <NAME> 127.0.01/<PORT>
/connect <NAME> -password=<PASWORD>
```

   
  
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
   
    
### RFC needs commands : ( [4. Message details](https://www.rfcreader.com/#rfc1459_line568) )
| command | params | description |
|---|---|---|
| PASS | `<password>` | used to set up a 'password connection'. a password must be set before any attempt. |
| NICK | `<nickname>` | used to assign an nickname to the user or change the previous one. there can be no two identical nicknames on the server. |
| USER | `<username>` `<hostname>` `<servername>` `<realname>` | used at the beginning of a connection to specify the username, hostname, server name and real name of the new user. |
|SERVER| |  X | not implemented (this server don't manage server to server communication) |
| OPER | `<user>` `<password>` | used to take operator rights. parameters are used to identify the user. |
| QUIT | `[<quit message>]` | used to terminate the connection to the server. the server should end the connection with the client when it sees the sent message. |
| SQUIT | X | not implemented (this server don't manage server to server communication) |
| JOIN | `<channel>{,<channel>}` `[<key>{,<key>}]` | used by the client to enter the channel. if a password is set, it must be correct. when users enter a channel, they will receive a notification about all users on the channel. if there was no group before, then the group is created. |
| PART | `<channel>{,<channel>}` | used by the user to leave the channels that he will specify in the parameters. |
| MODE | | idk |
| TOPIC | `<channel>` `[<topic>]` | used to edit or view a channel topic. the channel topic will remain the same unless a new topic is set. |
| NAMES | `[<channel>{,<channel>}]` | used by a user can get a list of all users in the channel. |
| LIST | `[<channel>{,<channel>}` `[<server>]]` | used to display a list of channels and their topics. if the channel parameter is used, then only the status of this channel is displayed. |
| INVITE | `<nickname>` `<channel>` | used to invite users to the channel. if the channel is open, then only the channel operator can invite to it. |
| KICK | `<channel>` `<user>` `[<comment>]` | used to exclude a user from a channel can only be used by the channel operator. |
| VERSION | | idk |
| STATS | | idk |
| LINKS | X | not implemented (this server don't manage server to server communication) |
| TIME | `<server>`  | The time message is used to query local time from the specified server. |
| CONNECT | X | not implemented (this server don't manage server to server communication) |
| TRACE | X | not implemented (this server don't manage server to server communication) |
| ADMIN |  | idk |
| INFO |  | idk |
| PRIVMSG | `<receiver>{,<receiver>}` `<text to be sent>` | used for private correspondence between users. also exists the ability to send messages to channels. |
| NOTICE | `<nickname>` `<text>` | works the same as PRIVMSG, except that no response is sent in response to a message. |
| WHO | `<name> [<o>]`| idk |
| WHOIS | | idk |
| WHOWAS | | idk |
| KILL | `<user>` `<comment>` | disconnects the user from the server. |
| PING / PONG | `<server1>` `[<server2>]` | used to check for client activity on the other end of the connection. this message is sent at regular intervals unless other activity is noticed from the connection. if the client does not send PONG in response to PING, the connection is closed. |
| ERROR | X | not implemented (this server don't manage server to server communication) |

Le client irc :
Envoi :
 - PASS password
 - NICK douwi
 - USER douwi 0 * :douwi

Descriptor 4 send : 11 bytes :
 - JOIN cool
 - NICK douwi
 - USER douwi 0 * :douwi

### Useful infos :
##### Ports :
1-65535 are available, and ports in range 1-1023 are the privileged ones: an application needs to be run as root in order to listen to these ports.



[TODO]
- trouver comment recup une commande nc
- recuperer correctement les args pr creation des users
- gerer, checker quand user ce deco quil soit sortie de tt ses chans
- parsing propre (si plusieurs commandes en une ligne et si plusieurs arg pr une meme cmd)
- implementer les commandes et les replies correspondantes
- implementer les topics (RFC 4.2.4)
- implementer et bien comprendre les modes selon la rfc 4.2.3.2 a 4.2.3.1 
- check tous ce qui peu etre passe en const et private

Check prefix etc : RFC 2.3 Messages

#### Modes : 
4.2.3.2 User modes
   Parameters: <nickname> {[+|-]|i|w|s|o}

   The user MODEs are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.
   A user MODE command may only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.

   The available modes are as follows:

           i - marks a users as invisible;
           s - marks a user for receipt of server notices;
           w - user receives wallops;
           o - operator flag.

4.2.3.1 Channel modes
   Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
               [<ban mask>]

   The MODE command is provided so that channel operators may change the
   characteristics of `their' channel.  It is also required that servers
   be able to change channel modes so that channel operators may be
   created.

   The various modes available for channels are as follows:

           o - give/take channel operator privileges;
           p - private channel flag;
           s - secret channel flag;
           i - invite-only channel flag;
           t - topic settable by channel operator only flag;
           n - no messages to channel from clients on the outside;
           m - moderated channel;
           l - set the user limit to channel;

           b - set a ban mask to keep users out;
           v - give/take the ability to speak on a moderated channel;
           k - set a channel key (password).

   When using the 'o' and 'b' options, a restriction on a total of three
   per mode command has been imposed.  That is, any combination of 'o' and