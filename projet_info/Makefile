NAME = library

SRCS_DIR = ./src
SRCS = $(wildcard $(SRCS_DIR)/*.c)

INC_DIR = ./inc

OBJS = $(SRCS:.c=.o)
OBJS_DIR = ./obj

CC = gcc
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -g -o $(NAME) $(OBJS)

%.o:%.c
	$(CC) $(FLAGS) -I$(INC_DIR) -o $@ -c $<

.PHONY : clean fclean 

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all