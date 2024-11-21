CC = gcc
.SUFFIXES: .c .h
SRC= $(wildcard *.c *.h)

.c:
	${CC} -std=c11 -pedantic -g -Wall -Wextra -pthread -o $@ $<