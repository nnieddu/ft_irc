# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ninieddu <ninieddu@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/22 17:20:49 by ninieddu          #+#    #+#              #
#    Updated: 2022/02/22 17:27:27 by ninieddu         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME	=	ft_irc

COMP 	=	c++

FLAG	=	-Wall -Wextra -Werror -std=c++98

SRC		=	main.cpp Server.cpp

INCS	=

OBJ		=	$(SRC:.cpp=.o)

%.o : %.cpp $(INCS)
	$(COMP) $(FLAG) -c $< -o $@

all : $(NAME) runv

$(NAME) : $(OBJ)
	$(COMP) $(FLAG) $(OBJ) -o $(NAME)

run : $(NAME)
	./$(NAME) 7005 password
	
runv : $(NAME)
	valgrind --track-origins=yes -s ./$(NAME) 7005 password

clean :
	rm -f $(OBJ)

fclean :
	rm -f $(NAME) $(OBJ)

re : fclean all

.PHONY : all clean fclean re run runv