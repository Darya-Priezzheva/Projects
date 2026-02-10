CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GREP_SRC = s21_grep.c
GREP_OBJ = s21_grep.o
GREP_BIN = s21_grep

all: $(GREP_BIN)

$(GREP_BIN): $(GREP_OBJ)
	$(CC) $(CFLAGS) $(GREP_OBJ) -o $(GREP_BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(GREP_OBJ)

fclean: clean
	rm -f $(GREP_BIN)

re: fclean all

.PHONY: all clean fclean re
