vinyl: vinyl.o inventory.o input.o
	gcc vinyl.o inventory.o input.o -o vinyl
inventory.o: inventory.c inventory.h input.h
	gcc -Wall -std=c99 -g -c -o inventory.o inventory.c
input.o: input.c input.h
	gcc -Wall -std=c99 -g -c -o input.o input.c
vinyl.o: vinyl.c inventory.h input.h
	gcc -Wall -std=c99 -g -c -o vinyl.o vinyl.c
clean:
	rm -f output.txt
	rm -f stderr.txt
	rm -f vinyl.o
	rm -f inventory.o
	rm -f input.o
	rm -f vinyl