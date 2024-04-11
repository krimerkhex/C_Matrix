WWW = -Wall -Wextra -Werror -std=c11
LINUX=-lsubunit -lrt -lpthread -lm
LIBS=-lcheck
LFLAGS=-fprofile-arcs -ftest-coverage
OS=$(shell uname -s)

all: clean unit_test gcov_report

re: clean all 

test: s21_matrix.a
ifeq ($(OS), Darwin)
	gcc $(WWW) test.c s21_matrix.a -o test $(LIBS)
else
	gcc $(WWW) test.c s21_matrix.a -o test $(LIBS) $(LINUX)
endif
	./test

unit_test: s21_matrix.a
ifeq ($(OS), Darwin)
	gcc $(WWW) unit_test.c s21_matrix.a -o unit_test $(LIBS)
else
	gcc $(WWW) unit_test.c s21_matrix.a -o unit_test $(LIBS) $(LINUX)
endif
	./unit_test

gcov_report: clean_test
ifeq ($(OS), Darwin)
	gcc $(WWW) -fprofile-arcs -ftest-coverage unit_test.c s21_*.c -o unit_test $(LIBS)
else
	gcc $(WWW) -fprofile-arcs -ftest-coverage unit_test.c s21_*.c -o unit_test $(LIBS) $(LINUX)
endif
	./unit_test
	lcov -t "unit_test" -o unit_test.info -c -d .
	genhtml -o report unit_test.info
	open report/index.html

s21_matrix.a: build_s21_matrix 

build_s21_matrix:
	gcc -c s21_*.c
	ar rcs s21_matrix.a s21_*.o
	ranlib s21_matrix.a

valgff: s21_matrix.a clean_obj
ifeq ($(OS), Darwin)
	gcc $(WWW) unit_test.c s21_matrix.a -o unit_test $(LIBS)
	make clean_obj
else
	gcc $(WWW) unit_test.c s21_matrix.a -o unit_test $(LIBS) $(LINUX)
	make clean_obj
endif
	valgrind --leak-check=full --track-origins=yes -s ./unit_test

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

install_brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

install_valgrind:
	brew install --HEAD LouisBrunner/valgrind/valgrind

install_cpch:
	brew install cppcheck

cpplint:
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extensions=c *.h
	python3 ../materials/linters/cpplint.py --extensions=c *.c
	rm -rf CPPLINT.cfg

clean_obj:
	rm -rf *.o

clean_lib: 
	rm -rf *.a

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf report

clean: clean_lib clean_lib clean_test clean_obj
	rm -rf s21_test
