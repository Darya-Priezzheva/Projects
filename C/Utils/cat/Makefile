CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

CAT_SRC = s21_cat.c
CAT_OBJ = s21_cat.o
CAT_BIN = s21_cat


all: $(CAT_BIN)

$(CAT_BIN): $(CAT_OBJ)
	$(CC) $(CFLAGS) $(CAT_OBJ) -o $(CAT_BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CAT_OBJ)

fclean: clean
	rm -f $(CAT_BIN)

re: fclean all

.PHONY: all clean fclean re
