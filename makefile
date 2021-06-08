
export CFLAGS = -I.

debug: .always 
	make libhashtable.a libhashtable.so CFLAGS="$(CFLAGS) -Og -g -Wall"

release: .always 
	make libhashtable.a libhashtable.so CFLAGS="$(CFLAGS) -O3 -Wall"

test: .always
	make debug 
	make test.exe CFLAGS="$(CFLAGS) -Og -g -Wall"
	gdb test.exe 
	#./test.exe

clean: .always 
	rm -f hashtable.o

test.exe: test.c libhashtable.so hashtable.h 
	gcc $(CFLAGS) test.c libhashtable.so -o test.exe 

.always:

hashtable.o: hashtable.c hashtable.h 
	gcc $(CFLAGS) -c hashtable.c -o hashtable.o

libhashtable.so: hashtable.o 
	gcc $(CFLAGS) -shared hashtable.o -o libhashtable.so

libhashtable.a: hashtable.o 
	ar r libhashtable.a hashtable.o
