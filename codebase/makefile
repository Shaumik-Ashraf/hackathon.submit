
all: main.c getdata.o mathstats.o crthtml.o
	gcc -o main.cgi main.c getdata.o mathstats.o crthtml.o
	
getdata.o: getdata.c getdata.h
	gcc -c getdata.c
	
mathstats.o: mathstats.c mathstats.h
	gcc -c mathstats.c -lm
	
crthtml.o: crthtml.c crthtml.h
	gcc -c crthtml.c
	
clean:
	rm *.o
	rm *.cgi

#makefile
