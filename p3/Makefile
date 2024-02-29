vcalc: vigesimal.o vcalc.o check.o
	gcc vigesimal.o vcalc.o check.o -o vcalc
vigesimal.o: vigesimal.c vigesimal.h 
	gcc -Wall -std=c99 -g -c vigesimal.c
check.o: check.c check.h
	gcc -Wall -std=c99 -g -c check.c
vcalc.o: vcalc.c 
	gcc -Wall -std=c99 -g -c vcalc.c
clean:
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt