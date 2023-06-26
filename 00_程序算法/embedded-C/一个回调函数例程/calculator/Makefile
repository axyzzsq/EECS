object = calc.o test.o
main : $(object)
	gcc -o main $(object)
%.o : %.c
	gcc -c $<

clean:
	rm *.o 
	rm main
