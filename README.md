# ft_irc
L’objectif de ce projet est de reproduire le fonctionnement d’un serveur IRC. 

Internet Relay Chat est un protocole de communication textuel sur Internet. 
Il sert à la communication instantanée principalement sous la forme de discussions en groupe par l’intermédiaire  
de canaux de discussion, mais peut aussi être utilisé pour de la communication directe entre deux personnes.
Les clients IRC se connectent à des serveurs IRC afin d’accéder à des canaux. Les
serveurs IRC sont connectés entre eux afin de créer des réseaux.

RFC 1459 Updated by RFC 2812  
 
RFC 2810 Internet Relay Chat: Architecture  
RFC 2811 Internet Relay Chat: Channel Management  
RFC 2812 Internet Relay Chat: Client Protocol  
RFC 2813 Internet Relay Chat: Server Protocol   
https://www.rfcreader.com/  
  
https://www.tutorialspoint.com/websockets/index.htm  
http://vidalc.chez.com/lf/socket.html  
http://www.codingbison.com/c/c-sockets-introduction.html
https://vichargrave.github.io/programming/tcp-ip-network-programming-design-patterns-in-cpp/
https://ncona.com/2019/04/building-a-simple-server-with-cpp/
https://www.youtube.com/watch?v=cNdlrbZSkyQ

https://www.irchelp.org/protocol/rfc/

https://www.alien.net.au/irc/
https://fr.wikipedia.org/wiki/Aide:IRC/commandes

### Usefuls :  
Weechat irc client for testing :  
apt-get install weechat-curses weechat-plugins 

netcat testing :
```
nc 127.0.01 <PORT>
```
### Docs :
#### Ports :
1-65535 are available, and ports in range 1-1023 are the privileged ones: an application needs to be run as root in order to listen to these ports.