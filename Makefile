SHELL:= /bin/bash

COMP = gcc -g $(LOGS) $(TESTING)
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
PROGECT_NAME_OUT = tetris.out

UNAME_S := $(shell uname -s)

PKG_FLAG = $(shell pkg-config --cflags --libs check)
# PKG_FLAG = $(shell pkg-config --cflags --libs check) -lm

LOGS = #-DLOGS
TESTING = #-DTESTING


all: $(lib_name)

play: $(PROGECT_NAME_OUT) 
	./$(PROGECT_NAME_OUT)

install: $(PROGECT_NAME_OUT)

uninstall: clean clean_test
	-rm -rf $(lib_name) tetrisHighScore.txt

rebuild: uninstall install

$(PROGECT_NAME_OUT): $(lib_name) $(FRONT_FILE_OBJ) $(MAIN_FILE_OBJ)
	$(COMP) $(FLAGS) $(MAIN_FILE_OBJ) $(FRONT_FILE_OBJ) $(lib_flag) -lncurses -o $(PROGECT_NAME_OUT) $(PKG_FLAG)

$(MAIN_FILE_OBJ): $(MAIN_FILE_SOURSE)
	$(COMP) $(FLAGS_C) $(LODS) $(MAIN_FILE_SOURSE) -o $(MAIN_FILE_OBJ) $(PKG_FLAG)
	# $(COMP) $(FLAGS_C) $(MAIN_FILE_SOURSE) -o $(MAIN_FILE_OBJ) $(PKG_FLAG)

$(FRONT_FILE_OBJ): $(FRONT_FILE_SOURSE)
	$(COMP) $(FLAGS_C) $(FRONT_FILE_SOURSE) -o $(FRONT_FILE_OBJ) $(PKG_FLAG)

test: $(lib_name) $(BACK_TEST_FILE_OBJ)
	$(COMP) $(BACK_TEST_FILE_OBJ) $(lib_flag) -lcheck  -lm $(PKG_FLAG)
	-./a.out

$(BACK_TEST_FILE_OBJ): $(BACK_TEST_FILE_C)
	$(COMP) $(FLAGS_C) $(BACK_TEST_FILE_C) -o $(BACK_TEST_FILE_OBJ)	$(PKG_FLAG)

$(BACK_TEST_FILE_C): $(BACK_TEST_FILE_SOURSE)
	checkmk $(BACK_TEST_FILE_SOURSE) > $(BACK_TEST_FILE_C)

$(lib_name):
	$(COMP) $(FLAGS_C) $(BACK_FILE_SOURSE) $(PKG_FLAG)
	ar rcs $(lib_name) *.o
	ranlib $(lib_name)
	rm -rf *.o

gcov_report: $(FILE_SOURSE) $(BACK_TEST_FILE_OBJ)
	$(COMP) $(FLAGS) -Wno-decimalop-truncation -Wno-memset-transposed-args -fprofile-arcs -ftest-coverage $(BACK_TEST_FILE_OBJ) $(BACK_FILE_SOURSE) -o gcov_report $(lib_flag) $(PKG_FLAG)
	./gcov_report
	lcov --directory . -t "gcov_report" -o gcov_report.info -c
	genhtml -o report gcov_report.info
	-rm -f *.gcno *.gcda gcov_report.info
	-open report/index.html

open: gcov_report
	lynx report/index.html

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

leaks: $(PROGECT_NAME_OUT)
	leaks -atExit -- ./$(PROGECT_NAME_OUT)

valgrind: test
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./a.out

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem main.c gui/cli/frontend.c brick_game/tetris/*.c

cl:
	-@cp ../materials/linters/.clang-format ../src/.clang-format; \
	clang-format -style=Google -i *.c */*/*.c */*/*.h */*/*/*.h; \
	rm .clang-format

valgrind_lin: test_lin
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --log-file=trash.log ./a.out
