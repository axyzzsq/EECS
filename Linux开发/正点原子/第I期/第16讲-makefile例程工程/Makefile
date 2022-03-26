main:main.o input.o calcu.o
	gcc -o main main.o input.o calcu.o
main.o:main.c
	gcc -c main.c
input.o:input.c
	gcc -c input.c
calcu.o:calcu.c
	gcc -c calcu.c
clean:
	rm *.o
	rm main
