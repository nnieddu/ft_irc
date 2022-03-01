# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/22 17:20:49 by ninieddu          #+#    #+#              #
#   Updated: 2022/02/28 16:47:01 by mrozniec         ###   ########.fr       # #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

COMP 	=	c++

FLAG	=	-Wall -Wextra -Werror -std=c++98 -g

SRCS	=	main.cpp	\
			Server.cpp	\
			Channel.cpp	\
			Socket.cpp	\
			User.cpp	\
			Command.cpp	\
			Commands.cpp\
			privmsg.cpp	\
			Topic.cpp	\
			Interpret.cpp

INCS	=	incs/Server.hpp		\
			incs/Channel.hpp	\
			incs/Socket.hpp		\
			incs/User.hpp		\
			incs/Command.hpp	\
			incs/Commands.hpp	\
			incs/Topic.hpp		\
			incs/Interpret.hpp

SRCS_DIR=	./srcs/
OBJ_DIR	=	./srcs/obj/
OBJ		=	$(addprefix $(OBJ_DIR),$(SRCS:.cpp=.o))

all :
	mkdir -p $(OBJ_DIR)
	$(MAKE) $(NAME)
##	$(MAKE) runv

$(OBJ_DIR)%.o:$(SRCS_DIR)%.cpp $(INC)
	$(COMP) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJ) $(INCS)
	$(COMP) $(FLAG) $(OBJ) -o $(NAME)

runv : $(NAME)
	valgrind --leak-check=full ./$(NAME) 7005 password
#| cat -e

clean :
	rm -rf $(OBJ_DIR)

fclean :
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re runv