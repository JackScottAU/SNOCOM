snocom: snocom.o
	gcc -o snocom $^

clean:
	rm snocom
	rm *.o

%.o: %.c
	gcc -c $< -o $@