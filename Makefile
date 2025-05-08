# === VARIABLES GLOBALES ===

NAME = libft_malloc_$(HOSTTYPE).so
SRCS_DIR = srcs/
OBJ_DIR = obj/
SRC_FILES = malloc.c\
			free.c\
			realloc.c\
			show_alloc_mem.c\
			show_alloc_mem_ex.c\
			utils.c\
			debug.c
INCLUDES = -I includes -I libft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_OBJS = $(shell find $(LIBFT_DIR) -name '*.o')

TEST_NAME = test_malloc
TEST_SCRIPT = ./run_test.sh
TEST_DIR = tests/
TEST_SRC = $(wildcard $(TEST_DIR)*.c)
TEST_OBJ = $(patsubst $(TEST_DIR)%.c, $(OBJ_DIR)%.o, $(TEST_SRC))
TEST_HEADERS = $(wildcard $(TEST_DIR)*.h)

CC = gcc
DEBUG_FLAGS ?=
CFLAGS = -Wall -Wextra -Werror -fPIC $(DEBUG_FLAGS)
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
	@$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJS) $(LIBFT_OBJS)
	@echo "$(GREEN)✅ Bibliothèque partagée $(NAME) créée$(RESET)"

link:
	@ln -sf $(NAME) libft_malloc.so
	@echo "$(CYAN)🔗 Lien symbolique libft_malloc.so créé$(RESET)"

$(LIBFT):
	@make -C $(LIBFT_DIR) DEBUG_FLAGS="$(DEBUG_FLAGS)"

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# === TEST ===

$(OBJ_DIR)%.o: $(TEST_DIR)%.c $(TEST_HEADERS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(TEST_OBJ) $(LIBFT) $(NAME) link
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(TEST_OBJ) -L . -lft_malloc
	clear
	@echo "$(YELLOW)🚀 Test compilé$(RESET)"

run: test
	@echo "$(CYAN)▶️  Exécution du test :$(RESET)"
	@LD_LIBRARY_PATH=. ./$(TEST_NAME)

script:
	@$(TEST_SCRIPT) $(TEST_NAME)

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

.PHONY: all clean fclean re link test script
