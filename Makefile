# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/04 11:00:31 by fmaury            #+#    #+#              #
#    Updated: 2019/08/09 14:18:54 by cbarbier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= woody_woodpacker

COMPILER		= gcc

CC_FLAGS		= -Wall -Werror -Wextra

INC				= inc

SRC_DIR			= src
OBJ_DIR			= obj

SRC				= main.c			\
				  load.c			\
				  err.c
					
OBJ				= $(SRC:.c=.o)

SRCS			= $(addprefix $(SRC_DIR)/, $(SRC))
OBJS			= $(addprefix $(OBJ_DIR)/, $(OBJ))

all : $(NAME)

$(NAME): $(OBJS) $(INC)
	$(COMPILER) $(CC_FLAGS) $(OBJS) -o $(NAME)
	@echo " / \   / \   / \   / \   / \   / \ "
	@echo "( \033[0;32mW\033[0m ) ( \033[0;32mO\033[0m ) ( \033[0;32mO\033[0m ) ( \033[0;32mD\033[0m ) ( \033[0;32mY\033[0m ) ( \033[0;32m.\033[0m )"
	@echo " \_/   \_/   \_/   \_/   \_/   \_/ "

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)/woody.h
	@mkdir -p $(OBJ_DIR)
	$(COMPILER) $(CC_FLAGS) -I $(INC) -c $< -o $@

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
