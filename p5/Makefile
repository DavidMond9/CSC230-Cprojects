hash: hash.o sha256.o sha256constants.o
	gcc hash.o sha256.o sha256constants.o -o hash
sha256test: sha256test.o sha256.o sha256constants.o
	gcc sha256test.o sha256.o sha256constants.o -o sha256test
hash.o: hash.c sha256.h sha256constants.h
	gcc -Wall -std=c99 -g -c -o hash.o hash.c
sha256.o: sha256.c sha256.h sha256constants.h
	gcc -Wall -std=c99 -g -c -o sha256.o sha256.c
sha256constants.o: sha256constants.c sha256constants.h
	gcc -Wall -std=c99 -g -c -o sha256constants.o sha256constants.c
sha256test.o: sha256test.c sha256.h sha256constants.h
	gcc -Wall -std=c99 -g -c -o sha256test.o sha256test.c
clean: 
	-rm -f *.o
	-rm -f sha256test
	-rm -f hash
	-rm -f sha256