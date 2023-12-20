# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: seyildir <seyildir@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/12/20 14:23:03 by seyildir      #+#    #+#                  #
#    Updated: 2023/12/20 14:23:04 by seyildir      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME	= philo

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -pthread -g -I./
COMPILE = $(CC) $(CFLAGS)

RM		= rm -f

SRC_DIR	= src/
OBJ_DIR	= obj/

SRC		= $(wildcard $(SRC_DIR)*.c)
OBJ		= $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	@echo $@ ready.
	@$(COMPILE) $(OBJ) -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo Creating $(notdir $(basename $@))
	@$(COMPILE) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@echo objects deleting...
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo $(NAME) deleting...
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re