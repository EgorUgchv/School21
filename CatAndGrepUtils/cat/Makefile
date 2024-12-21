all: s21_cat

s21_cat:
	gcc -Wall -Werror -Wextra -std=c11 -pedantic s21_cat.c s21_cat.h -o s21_cat

format:
	clang-format -i *.c *.h

rebuild:
	make clean
	make all

test: s21_cat
	bash test.sh

clean:
	rm -f s21_cat

