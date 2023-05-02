CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

project: project.o imageManip.o ppm_io.o
	$(CC) -o project project.c ppm_io.o imageManip.o -lm

project.o: project.c imageManip.h ppm_io.h
	$(CC) $(CFLAGS) -c project.c

imageManip.o: ppmIO.o imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c

ppmIO.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c


clean:
	rm -f *.o project
