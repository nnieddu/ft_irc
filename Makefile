NAME	=	ircserv

COMP 	=	c++

FLAG	=	-Wall -Wextra -Werror -std=c++98 -g

SRCS	=	main.cpp	\
			Server.cpp	\
			Channel.cpp	\
			Socket.cpp	\
			User.cpp	\
			Command.cpp	\
			Interpret.cpp

INCS	=	incs/Server.hpp		\
			incs/Channel.hpp	\
			incs/Socket.hpp		\
			incs/User.hpp		\
			incs/Command.hpp	\
			incs/Interpret.hpp

SRCS_DIR=	./srcs/
OBJ_DIR	=	./srcs/obj/
OBJ		=	$(addprefix $(OBJ_DIR),$(SRCS:.cpp=.o))

CMD_DIR	=	./srcs/Commands/
CMDLIB	=	$(addprefix $(CMD_DIR),cmdlib.a)

all :
	mkdir -p $(OBJ_DIR)
	$(MAKE) -C $(CMD_DIR) --no-print-directory
	$(MAKE) $(NAME)

$(OBJ_DIR)%.o:$(SRCS_DIR)%.cpp $(INCS)
	$(COMP) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJ) $(INCS) $(CMDLIB)
	$(COMP) $(FLAG) $(OBJ) $(CMDLIB) -o $(NAME)

run : all
	./$(NAME) 7005 password

runl : all
	leaks -atExit -- ./$(NAME) 7005 password 

runv : re 
	valgrind --leak-check=full ./$(NAME) 7005 password
#| cat -e

clean :
	$(MAKE) clean -C $(CMD_DIR) --no-print-directory
	rm -rf $(OBJ_DIR)

fclean :
	$(MAKE) fclean -C $(CMD_DIR) --no-print-directory
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re run runl runv