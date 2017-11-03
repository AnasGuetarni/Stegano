#gcc -Wall -Wextra -std=gnu11 prg.c -o prg -lpthread

install: prg.o
	gcc prg.o -o prg -lpthread

prg.o: prg.c
	gcc -Wall -std=gnu11 -c prg.c -lphthread

clean:
	rm -f *.o prg
