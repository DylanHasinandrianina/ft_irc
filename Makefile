CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP -Iincludes
RM = rm -f
SRCS = main.cpp server.cpp user.cpp command.cpp channels.cpp dispatch.cpp \
		pass.cpp
OBJS_DIR = objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))
DEP = $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.d))
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(OBJS) -o $@

$(OBJS_DIR)/%.o: %.cpp
		mkdir -p $(OBJS_DIR)
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) -r $(OBJS_DIR) $(DEP)

fclean: clean
		$(RM) $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
