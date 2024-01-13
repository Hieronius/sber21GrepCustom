# Makefile for s21_grep

FLAGS=-Wall -Werror -Wextra

all: s21_grep

s21_grep: grep.o
	gcc $(FLAGS) grep.c -o s21_grep

test: grep_test.sh
	bash grep_test.sh

grep.o: grep.c
	gcc $(FLAGS) -c grep.c

clean:
	rm -rf *.o
	rm -rf s21_grep
	rm -rf test
