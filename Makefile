

all:
	gcc *.c -o trabalho1 -g

run:
	./trabalho1

zip:
	zip trabalho1.zip *.c *.h Makefile