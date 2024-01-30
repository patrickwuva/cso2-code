all: executable

foo.o:
	gcc -Wall -c foo.c

main.c:
	gcc -Wall -c main.c

executable: foo.o main.c
	gcc -Wall -o executable foo.o main.c

clean:
	rm --force main.o foo.o executable
