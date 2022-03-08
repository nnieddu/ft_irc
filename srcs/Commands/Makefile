NAME	=	cmdlib.a

COMP	=	c++
AR	 	=	ar rc

FLAG	=	-Wall -Wextra -Werror -std=c++98 -g

SRCS	=	Cap.cpp		\
			Info.cpp	\
			Invite.cpp	\
			Join.cpp	\
			Kick.cpp	\
			List.cpp	\
			Mode.cpp	\
			Names.cpp	\
			Nick.cpp	\
			Notice.cpp	\
			Part.cpp	\
			Pass.cpp	\
			Ping.cpp	\
			Pong.cpp	\
			Privmsg.cpp	\
			Quit.cpp	\
			Oper.cpp	\
			Topic.cpp	\
			Time.cpp	\
			User.cpp	\
			Version.cpp	\
			Whois.cpp

INCS	=	../../incs/Server.hpp	\
			../../incs/Channel.hpp	\
			../../incs/Socket.hpp	\
			../../incs/User.hpp		\
			../../incs/Command.hpp	\
			../../incs/Commands.hpp

OBJ_DIR	=	./obj/
OBJ		=	$(addprefix $(OBJ_DIR),$(SRCS:.cpp=.o))

all :
	mkdir -p $(OBJ_DIR)
	$(MAKE) $(NAME)

$(OBJ_DIR)%.o:$(SRCS_DIR)%.cpp $(INCS)
	$(COMP) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJ) $(INCS)
	$(AR) $(NAME) $(OBJ) $(INCS)

clean :
	rm -rf $(OBJ_DIR)

fclean :
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)

.PHONY : all clean fclean