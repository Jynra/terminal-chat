CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -I./include
NAME = chat_app

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/client/client.c \
       $(SRC_DIR)/client/client_handlers.c \
       $(SRC_DIR)/server/server.c \
       $(SRC_DIR)/server/server_handlers.c \
       $(SRC_DIR)/network/socket.c \
       $(SRC_DIR)/network/connection.c \
       $(SRC_DIR)/utils/key_generator.c \
       $(SRC_DIR)/utils/logger.c \
       $(SRC_DIR)/utils/error_handler.c \
       $(SRC_DIR)/utils/display.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: directories $(NAME)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/client
	@mkdir -p $(OBJ_DIR)/server
	@mkdir -p $(OBJ_DIR)/network
	@mkdir -p $(OBJ_DIR)/utils

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re directories