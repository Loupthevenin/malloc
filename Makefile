# === VARIABLES GLOBALES ===

NAME = libft_malloc_$(HOSTTYPE).so
SRCS_DIR = srcs/
OBJ_DIR = obj/
SRC_FILES = malloc.c\
			free.c\
			realloc.c\
			show_alloc_mem.c
INCLUDES = -I includes -I libft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_OBJS = $(shell find $(LIBFT_DIR) -name '*.o')

TEST_NAME = test_malloc
TEST_SCRIPT = ./run_test.sh
TEST_DIR = tests/
TEST_SRC = $(TEST_DIR)test_malloc.c
TEST_OBJ = $(OBJ_DIR)test_malloc.o

CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
AR = ar rc
RM = rm -rf

# === COULEURS ===

GREEN       = \033[0;32m
MAGENTA     = \033[1;35m
YELLOW      = \033[1;33m
CYAN        = \033[1;36m
RED         = \033[0;31m
RESET       = \033[0m

ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

SRCS = $(shell find $(SRC_DIR) -name '*.c' | xargs basename -a)
OBJS = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

all: $(LIBFT) $(NAME) link

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) -shared -o $(NAME) $(OBJS) $(LIBFT_OBJS)
	@echo "$(GREEN)✅ Bibliothèque partagée $(NAME) créée$(RESET)"

link:
	@ln -sf $(NAME) libft_malloc.so
	@echo "$(CYAN)🔗 Lien symbolique libft_malloc.so créé$(RESET)"

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# === TEST ===

$(TEST_OBJ): $(TEST_SRC)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(TEST_OBJ) $(LIBFT) $(NAME) link
	@$(CC) -o $(TEST_NAME) $(TEST_OBJ) -L . -lft_malloc
	@echo "$(YELLOW)🚀 Test compilé$(RESET)"

run: test
	@echo "$(CYAN)▶️  Exécution du test :$(RESET)"
	@LD_LIBRARY_PATH=. ./$(TEST_NAME)

script: test
	@echo "$(MAGENTA)🔍 Exécution avec Valgrind :$(RESET)"
	@$(TEST_SCRIPT) $(TEST_NAME)

run_all: run script

# === CLEANUP ===

clean:
	@make clean -C $(LIBFT_DIR)
	@$(RM) $(OBJ_DIR)
	@echo "$(YELLOW)🧹 Clean terminé$(RESET)"

fclean:
	@make fclean -C $(LIBFT_DIR)
	@$(RM) $(OBJ_DIR) $(NAME) $(TEST_NAME) libft_malloc.so
	@echo "$(RED)🧨 Full clean effectué$(RESET)"

re: fclean all

.PHONY: all clean fclean re link test
