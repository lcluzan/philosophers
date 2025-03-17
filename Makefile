NAME			=	philo
SHELL			=	/bin/bash

CC				=	cc
CFLAGS			=	-g -Wall -Wextra -Werror

SRCS_DIR		=	srcs/
C_SRCS_LIST		=	main.c init.c utils.c time.c routine.c monitor.c cleanup.c
C_SRCS			=	$(addprefix $(SRCS_DIR), $(C_SRCS_LIST))

OBJS_DIR		=	objs/
OBJS_LIST		:=	$(patsubst %.c, %.o, $(C_SRCS_LIST))
OBJS			=	$(addprefix $(OBJS_DIR), $(OBJS_LIST))

HEADERS			=	-I ./includes

# Couleurs
GREEN			=	\e[0;32m
YELLOW			=	\e[0;33m
BLUE			=	\e[0;34m
RED				=	\e[0;31m
RESET			=	\e[0m
BOLD			=	\e[1m

.PHONY : all clean fclean re

all : $(NAME)
	@echo -e "$(GREEN)$(BOLD)✓ Projet philo compilé avec succès !$(RESET)"

$(NAME) : $(OBJS)
	@echo -e "$(BLUE)➤ Création de l'exécutable...$(RESET)"
	@$(CC) $(CFLAGS) $^ -o $@
	@echo -e "$(GREEN)➤ Exécutable $(BOLD)$(NAME)$(RESET)$(GREEN) créé !$(RESET)"

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c includes/*.h
	@mkdir -p $(OBJS_DIR)
	@echo -e "$(YELLOW)➤ Compilation de $<...$(RESET)"
	@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

clean :
	@echo -e "$(RED)➤ Suppression des fichiers objets...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@echo -e "$(RED)➤ Fichiers objets supprimés !$(RESET)"

fclean : clean
	@echo -e "$(RED)➤ Suppression de l'exécutable $(NAME)...$(RESET)"
	@rm -rf $(NAME)
	@echo -e "$(RED)➤ Exécutable supprimé !$(RESET)"

re : fclean all
	@echo -e "$(GREEN)$(BOLD)✓ Projet recompilé avec succès !$(RESET)"
