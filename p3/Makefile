vcalc: vigesimal.o vcalc.o check.o
	gcc vigesimal.o vcalc.o check.o -o vcalc
vigesimal.o: vigesimal.c vigesimal.h check.h
	gcc -Wall -std=c99 -g -c -o vigesimal.o vigesimal.c
check.o: check.c check.h
	gcc -Wall -std=c99 -g -c -o check.o check.c 
vcalc.o: vcalc.c vigesimal.h check.h
	gcc -Wall -std=c99 -g -c -o vcalc.o vcalc.c 
clean:
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt
	rm -f vcalc.o
	rm -f vigesimal.o
	rm -f check.o