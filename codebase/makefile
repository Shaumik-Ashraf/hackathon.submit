
all: main.c getdata.o mathstats.o 
	gcc -w -o main.cgi main.c getdata.o mathstats.o -lm
	
getdata.o: getdata.c getdata.h
	gcc -w -c getdata.c
	
mathstats.o: mathstats.c mathstats.h
	gcc -c mathstats.c -lm
	
	
clean:
	touch t.o t.cgi
	rm *.o
	rm *.cgi

#makefile
