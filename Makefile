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
	./$(NAME)
	
runv : $(NAME)
	valgrind ./$(NAME)

clean :
	rm -f $(OBJ)

fclean :
	rm -f $(NAME) $(OBJ)

re : fclean all

.PHONY : all clean fclean re run runv