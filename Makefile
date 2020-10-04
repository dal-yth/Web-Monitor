NAME = web_monitor

CC = gcc

#FLAGS = -Wall -Wextra -Werror

MKDIR_P = mkdir -p

DIR_S = srcs

DIR_O = obj

SOURCES = main.c read_file.c url_append.c utils.c http_get_loop.c monitor_log.c log_info.c free.c

INC = inc

OBJ = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: obj_dir $(NAME)

#remember to add $(FLAGS) again to gcc...
$(NAME): $(OBJ)
	@echo "Creating executable..."
	@$(CC) -o $(NAME) $(OBJ)
	@echo "\e[1m$(NAME) created!\e[0m"

obj_dir:
	@echo "Creating object dir..."
	@${MKDIR_P} ${DIR_O}

$(DIR_O)/%.o: $(DIR_S)/%.c
	@$(CC) $(FLAGS) -I $(INC) -o $@ -c $<

clean:
	@echo "Removing project objects..."
	@rm -f $(OBJ)
	@echo "Removing object directory..."
	@rm -rf $(DIR_O)

fclean: clean
	@echo "Removing executable..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re obj_dir