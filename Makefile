CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude
SRC = $(wildcard src/*.c)
BUILD_DIR = build
OBJ = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC))
NAME = myshell

all: $(NAME)

# Crée le dossier build s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Lien de l'exécutable
$(NAME): $(BUILD_DIR) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Compilation des .o dans build/
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)
