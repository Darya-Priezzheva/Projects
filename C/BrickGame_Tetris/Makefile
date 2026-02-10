CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -std=c11
LDFLAGS = -lncurses
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
ifeq ($(shell uname), Linux)
        CHECK_LIBS := -lcheck -lm -lpthread -lsubunit
endif
ifeq ($(shell uname), Darwin)
        CHECK_LIBS := $(shell pkg-config --cflags --libs check) -lcheck -lm -lpthread
endif

LIB =  libtetris.a
GAME = tetris

SRC_DIR = brick_game/tetris
GUI_DIR = gui/cli
OBJ_DIR = obj

SRC_LIB = $(wildcard $(SRC_DIR)/*.c)
OBJ_LIB = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_LIB))

SRC_GUI = $(wildcard $(GUI_DIR)/*.c)
OBJ_GUI = $(patsubst $(GUI_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_GUI))

PREFIX = $(HOME)/Desktop/Tetris

TESTS = tests.c
TESTS_EXE = tests/test_run
TESTS_DIR = tests

DOC_SRC = dvi/tetris.md
DOC_DIR = doc

DIST_DIR = dist

.PHONY: all install uninstall dvi dist test gcov_report valgrind clang-format clean

all: $(LIB) $(GAME)

$(LIB): $(OBJ_LIB)
	ar rcs $@ $^

$(GAME): $(OBJ_GUI) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(OBJ_GUI) -L. -ltetris $(LDFLAGS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(GUI_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

install: all
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/lib
	cp $(GAME) $(PREFIX)/bin/
	cp $(LIB) $(PREFIX)/lib/

uninstall:
	rm -f $(PREFIX)/bin/$(GAME)
	rm -f $(PREFIX)/bin/score.txt
	rm -f $(PREFIX)/lib/$(LIB)


dvi:
	mkdir -p $(DOC_DIR)
	pandoc dvi/tetris.md -o doc/tetris.html
	xdg-open doc/tetris.html || open doc/tetris.html


dist: install
	mkdir -p $(DIST_DIR)
	tar -czvf $(DIST_DIR)/tetris.tar.gz Makefile brick_game gui doc

test: $(LIB) $(TESTS)
	mkdir -p $(TESTS_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $(TESTS_EXE) $(TESTS) -L. -ltetris $(CHECK_LIBS)
	./$(TESTS_EXE)

gcov_report: test
	./$(TESTS_EXE)
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*/test/*' '*.check' -o coverage_filtered.info --ignore-errors unused
	genhtml coverage_filtered.info --output-directory gcov_report
	@echo "Открываю отчёт в браузере..."
	@xdg-open ./gcov_report/index.html 2>/dev/null || open ./gcov_report/index.html 2>/dev/null || true


valgrind:
	valgrind --leak-check=full --suppressions=ncurses.supp  ./tetris

clang-format:
	cp ../materials/linters/.clang-format .
	clang-format -n $(SRC_DIR)/*.c $(GUI_DIR)/*.c

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(GAME) $(LIB) $(TESTS_DIR) $(DOC_DIR) $(DIST_DIR)
	rm -rf gcov_report *.info
	rm -f .clang-format
