CC = gcc
.SUFFIXES: .c .h
SRC= $(wildcard *.c *.h)

.c:
	${CC} -std=c11 -pedantic -Wall -Wextra -pthread -o $@ $<


clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC) documentation/html