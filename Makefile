# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/22 17:20:49 by ninieddu          #+#    #+#              #
#    Updated: 2022/02/24 10:27:29 by ninieddu         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

COMP 	=	c++

FLAG	=	-Wall -Wextra -Werror -std=c++98

SRCS	=	srcs/main.cpp  \
			srcs/Server.cpp \
			srcs/Socket.cpp \
			srcs/User.cpp \
			srcs/Commands.cpp

INCS	=	incs/Server.hpp \
			incs/Socket.hpp \
			incs/User.hpp \
			incs/Commands.hpp

OBJ		=	$(SRCS:.cpp=.o)

%.o : %.cpp $(INCS)
	$(COMP) $(FLAG) -c $< -o $@

all : $(NAME) runv

$(NAME) : $(OBJ)
	$(COMP) $(FLAG) $(OBJ) -o $(NAME)
	
runv : $(NAME)
	valgrind ./$(NAME) 7005 password

clean :
	rm -f $(OBJ)

fclean :
	rm -f $(NAME) $(OBJ)

re : fclean all

.PHONY : all clean fclean re runv