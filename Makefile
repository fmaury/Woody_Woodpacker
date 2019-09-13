# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cbarbier <cbarbier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/04 11:00:31 by fmaury            #+#    #+#              #
#    Updated: 2019/08/27 17:01:51 by cbarbier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= woody_woodpacker

COMPILER		= gcc

CC_FLAGS		= -Wall -Werror -Wextra

INC				= ./inc

LIB				= ./libft/libft.a
LIB_INC			= ./libft/includes

SRC_DIR			= ./src
ASM_SRC_DIR		= ./asm_src
OBJ_DIR			= ./obj

SRC				= main.c			\
				  load.c			\
				  utils.c			\
				  check.c			\
				  handle_elf64.c	\
				  dispatcher.c		\
				  chk_ptr.c 		\
				  cypher.c			\
				  err.c

ASM_SRC			= xor42.s			\
				  rot13.s			\
				  rc4.s

OBJ				= $(SRC:.c=.o)
OBJ				+= $(ASM_SRC:.s=.o)

SRCS			= $(addprefix $(SRC_DIR)/, $(SRC))
OBJS			= $(addprefix $(OBJ_DIR)/, $(OBJ))


all : $(NAME)

$(NAME): $(LIB) $(OBJS) $(INC)
	$(COMPILER) $(CC_FLAGS) -I $(LIB_INC) -I $(INC) $(OBJS) -o $(NAME) -L ./libft -lft
	@echo " / \   / \   / \   / \   / \   / \ "
	@echo "( \033[0;32mW\033[0m ) ( \033[0;32mO\033[0m ) ( \033[0;32mO\033[0m ) ( \033[0;32mD\033[0m ) ( \033[0;32mY\033[0m ) ( \033[0;32m.\033[0m )"
	@echo " \_/   \_/   \_/   \_/   \_/   \_/ "

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)/woody.h
	@mkdir -p $(OBJ_DIR)
	$(COMPILER) $(CC_FLAGS) -I$(INC) -I$(LIB_INC) -c $< -o $@

$(OBJ_DIR)/%.o: $(ASM_SRC_DIR)/%.s $(INC)/woody.h
	nasm -f elf64 $< -o $@

ifneq ($(shell make -q -C libft;echo $$?), 0)
.PHONY:	$(LIB)
endif

$(LIB):
	make -C libft

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJ_DIR)
	make -C libft clean

fclean: clean
	rm -rf $(NAME)
	rm -rf $(LIB)
	rm -rf woody

re: fclean all

test:
	./$(NAME) a.out && ./woody
	./$(NAME) /bin/ls && ./woody


.PHONY: all clean fclean re test