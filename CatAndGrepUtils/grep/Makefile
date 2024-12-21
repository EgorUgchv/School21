all: s21_grep

s21_grep:
	gcc -Wall -Werror -Wextra -std=c11 -pedantic -D_GNU_SOURCE s21_grep.c s21_grep.h -o s21_grep

format:
	clang-format -i *.c *.h

rebuild:
	make clean
	make all

test: s21_grep
	bash test.sh

clean:
	rm s21_grep
