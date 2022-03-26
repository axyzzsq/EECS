test: a.o b.o c.o
	gcc -o test $^
	
%.o : %.c
	gcc -c -o $@ $<

clean:
	rm *.o test

.PHONY: clean	