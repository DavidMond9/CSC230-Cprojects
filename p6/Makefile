CC = gcc
CFLAGS += -Wall -std=c99
LDLIBS += -lm

driver: driver.o map.o value.o input.o
	$(CC) driver.o map.o value.o input.o -o driver $(LDLIBS)
driver.o: driver.c input.h map.h value.h
	$(CC) $(CFLAGS) -g -c -o driver.o driver.c
map.o: map.c map.h value.h 
	$(CC) $(CFLAGS) -g -c -o map.o map.c
value.o: value.c value.h 
	$(CC) $(CFLAGS) -g -c -o value.o value.c
input.o: input.c input.h
	$(CC) $(CFLAGS) -g -c -o input.o input.c
doubleTest: doubleTest.o value.o
	$(CC) doubleTest.o value.o -o doubleTest $(LDLIBS)
doubleTest.o: doubleTest.c value.h 
	$(CC) $(CFLAGS) -g -c -o doubleTest.o doubleTest.c
stringTest: stringTest.o value.o
	$(CC) stringTest.o value.o -o stringTest $(LDLIBS)
stringTest.o: stringTest.c value.h
	$(CC) $(CFLAGS) -g -c -o stringTest.o stringTest.c
mapTest: mapTest.o value.o map.o
	$(CC) mapTest.o value.o map.o -o mapTest $(LDLIBS)
mapTest.o: mapTest.c value.h map.h
	$(CC) $(CFLAGS) -g -c -o mapTest.o mapTest.c 
clean: 
	-rm -f *.o
	-rm -f doubleTest
	-rm -f integerTest
	-rm -f mapTest
	-rm -f stringTest
	-rm -f output.txt
	-rm -f stderr.txt 
	-rm -f driver
	-rm -f *.gcda
	-rm -f *.gcno
	-rm -f *.gcov