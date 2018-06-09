http : *.o
	gcc -o http -pthread *.o 

*.o : *.c
	gcc -c *.c

.PHONY :clean
clean:
	\rm -rf *o ./http
