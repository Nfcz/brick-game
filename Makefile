SHELL:= /bin/bash

COMP = gcc -g
FLAGS = -Werror -Wextra -Wall -std=c11
FLAGS_C = $(FLAGS) -c  

BACK_FILE_SOURSE = brick_game/tetris/*.c

BACK_TEST_FILE_SOURSE = brick_game/tetris/test/test_s21_tetris.check
BACK_TEST_FILE_C      = brick_game/tetris/test/test_s21_tetris.c
BACK_TEST_FILE_OBJ    = brick_game/tetris/test/test_s21_tetris.o

lib_name = s21_tetris.a
lib_flag = -L. s21_tetris.a

FRONT_FILE_SOURSE = gui/cli/frontend.c
FRONT_FILE_OBJ = gui/cli/frontend.o

MAIN_FILE_SOURSE = main.c
MAIN_FILE_OBJ = main.o

PROGECT_NAME_DIST = brick_game_1.tar.gz

UNAME_S := $(shell uname -s)

TEST_LIB_FLAGS = $(shell pkg-config --cflags --libs check)
# TEST_LIB_FLAGS = $(shell pkg-config --cflags --libs check) -lm


#  all, install, uninstall, clean, dvi, dist, test, gcov_report
all: clean_test clean test

play: game.out
	./game.out

install: game.out

uninstall: clean clean_test
	-rm -rf s21_tetris.a

rebuild: uninstall install

game.out: $(lib_name) $(FRONT_FILE_OBJ) $(MAIN_FILE_OBJ)
	$(COMP) $(FLAGS) $(MAIN_FILE_OBJ) $(FRONT_FILE_OBJ) $(lib_flag) $(TEST_LIB_FLAGS) -lncurses -o game.out

$(MAIN_FILE_OBJ): $(MAIN_FILE_SOURSE)
	$(COMP) $(FLAGS_C) $(MAIN_FILE_SOURSE) -o $(MAIN_FILE_OBJ)

$(FRONT_FILE_OBJ): $(FRONT_FILE_SOURSE)
	$(COMP) $(FLAGS_C) $(FRONT_FILE_SOURSE) -o $(FRONT_FILE_OBJ)

test: $(lib_name) $(BACK_TEST_FILE_OBJ)
	$(COMP) $(BACK_TEST_FILE_OBJ) $(lib_flag) -lcheck
	-./a.out

$(BACK_TEST_FILE_OBJ): $(BACK_TEST_FILE_C)
	$(COMP) $(FLAGS_C) $(BACK_TEST_FILE_C) -o $(BACK_TEST_FILE_OBJ)	

$(BACK_TEST_FILE_C): $(BACK_TEST_FILE_SOURSE)
	checkmk $(BACK_TEST_FILE_SOURSE) > $(BACK_TEST_FILE_C)

$(lib_name):
	$(COMP) $(FLAGS_C) $(BACK_FILE_SOURSE)
	ar rcs $(lib_name) *.o
	ranlib $(lib_name)
	rm -rf *.o

gcov_report: $(FILE_SOURSE) $(BACK_TEST_FILE_OBJ)
	$(COMP) $(FLAGS) -Wno-decimalop-truncation -Wno-memset-transposed-args -fprofile-arcs -ftest-coverage $(BACK_TEST_FILE_OBJ) $(BACK_FILE_SOURSE) -o gcov_report $(lib_flag) $(TEST_LIB_FLAGS)
	./gcov_report
	lcov --directory . -t "gcov_report" -o gcov_report.info -c
	genhtml -o report gcov_report.info
	-rm -f *.gcno *.gcda gcov_report.info
	-open report/index.html

dvi: Doxyfile
	doxygen
	open documentation/dvi/html/index.html

dist:
	tar --exclude='*.o' --exclude='documentation/dvi' -cvzf $(PROGECT_NAME_DIST) main.c Makefile Doxyfile brick_game gui documentation

distclean:
	-rm -rf $(PROGECT_NAME_DIST)

clean:
	-rm -rf *.o */*/*.o */*/*/*.o gcov_report *.out report *.gcno *.gcda gcov_report.dSYM documentation/dvi

clean_test:
	-rm -rf $(BACK_TEST_FILE_C)

leaks: test
	leaks -atExit -- ./a.out

valgrind: test
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./a.out

cl:
	-@cp ../materials/linters/.clang-format ../src/.clang-format; \
	clang-format -style=Google -i *.c */*/*.c */*/*.h; \
	rm .clang-format

# test_lin: $(lib_name)
# 	$(COMP) $(TEST_FILE_OBJ) -o a.out $(lib_decimal_flag) $(TEST_LIB) 

# $(TEST_FILE_OBJ): $(TEST_FILE_C)
# 	$(COMP) $(FLAGS_C) -Wno-stringop-truncation -Wno-memset-transposed-args $(TEST_FILE_C) -o $(TEST_FILE_OBJ) $(TEST_LIB) 

# clean_for_linux:
# 	-rm -rf *.o */*.o a.out *.a s21_decimal.a

valgrind_lin: test_lin
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=trash.log ./a.out
# --tool=memcheck 
#	valgrind --leak-check=full 