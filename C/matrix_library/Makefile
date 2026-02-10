CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
ifeq ($(shell uname), Linux)
	CHECK_LIBS := -lcheck -lm -lpthread -lsubunit
endif
ifeq ($(shell uname), Darwin)
	CHECK_LIBS := $(shell pkg-config --cflags --libs check) -lcheck -lm -lpthread
endif

SRC = s21_matrix.c
OBJ = $(SRC:.c=.o)
LIB = s21_matrix.a

TEST = tests

all: $(LIB) test gcov_report

$(LIB): $(OBJ)
	ar rcs $@ $^
	rm -rf $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests.c: tests.check
	checkmk $< > $@

$(TEST): tests.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(CHECK_LIBS)

test: $(TEST)
	./$(TEST)

tests_cov: tests.c $(SRC)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $(TEST) $^ $(CHECK_LIBS)

gcov_report: tests_cov
	./$(TEST)
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*/test/*' '*.check' -o coverage_filtered.info --ignore-errors unused
	genhtml coverage_filtered.info --output-directory gcov_report
	@echo "Открываю отчёт в браузере..."
	@xdg-open ./gcov_report/index.html 2>/dev/null || open ./gcov_report/index.html 2>/dev/null || true

valgrind: $(TEST)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST)

clean:
	rm -f $(OBJ) $(LIB) $(TEST) tests.c *.gc* *.info
	rm -rf gcov_report

