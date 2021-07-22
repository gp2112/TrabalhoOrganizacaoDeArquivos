

all:
	gcc src/*.c -o trabalho1

run:
	./trabalho1

zip:
	zip trabalho1.zip src/* Makefile
