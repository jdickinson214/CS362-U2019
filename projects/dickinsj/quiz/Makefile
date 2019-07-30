CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme -g testme.c $(CFLAGS) 

quiz: testme
	-./testme 
	gcov testme.c -b *> results.out
	cat testme.c.gcov >> results.out


clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so *.out testme quiz
