CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
ifeq ($(shell uname), Linux)
CHECK_LIBS := -lcheck -lm -lpthread -lsubunit
endif
ifeq ($(shell uname), Darwin)
CHECK_LIBS := $(shell pkg-config --cflags --libs check) -lcheck -lm -lpthread
endif

SRC = $(wildcard utils/*.c core/arithmetic/arithmetics.c core/converts/converts.c core/compares/compare.c core/other/other.c)
OBJ = $(SRC:.c=.o)

TESTS = test/*.c
LIB = s21_decimal.a
EXEC = test_exec

all: s21_decimal.a test gcov_report

s21_decimal.a: $(OBJ)
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  ONE SECOND, PLEASE!                    ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"
	ar rcs s21_decimal.a $^
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  BUILD COMPLETE!                        ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"

%.o: %.c
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  BUILDING YOUR LIBRARY ...              ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"
	$(CC) $(CFLAGS) -c $< -o $@

test: clean s21_decimal_coverage
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  Checking your code ...                 ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TESTS) -o $(EXEC) s21_decimal.a $(CHECK_LIBS)
	./$(EXEC)
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  YOU PASS THROUGH ALL TESTS             ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"

s21_decimal_coverage:
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  REBUILD YOUR CODE WITH CHECK UP.       ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"
	$(CC) $(CFLAGS) --coverage -c $(SRC)
	ar rcs s21_decimal.a *.o
	rm -f *.o
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  BUILD COMPLETE                         ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"

gcov_report: test
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  Forming test report ...                ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*/test/*' -o coverage_filtered.info --ignore-errors unused
	genhtml coverage_filtered.info --output-directory gcov_report
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  HTML report generated in               ┃\033[0m"
	@echo "\033[32m┃  ./gcov_report/index.html               ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"
	@echo "\033[32m┏=========================================┓\033[0m"
	@echo "\033[32m┃  OPENING IN DEFAULT BROWSER             ┃\033[0m"
	@echo "\033[32m┗=========================================┛\033[0m"
	@open ./gcov_report/index.html || xdg-open ./gcov_report/index.html || true

clean:
	@echo "\033[31m┏=========================================┓\033[0m"
	@echo "\033[31m┃  Deleting unnecessary files...          ┃\033[0m"
	@echo "\033[31m┗=========================================┛\033[0m"
	rm -f  $(TESTS)*.o *.a *.gc* *.info $(EXEC) $(OBJ)
	rm -rf gcov_report
	@echo "\033[31m┏=========================================┓\033[0m"
	@echo "\033[31m┃  UNNECESSARY FILES DELETED.             ┃\033[0m"
	@echo "\033[31m┗=========================================┛\033[0m"