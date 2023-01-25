Windowing: main.o Windowing.o Buffer.o
	gcc main.o -o Windowing

main.o: main.c Windowing.h
	gcc -c main.c

Windowing.o: Windowing.c Buffer.h Windowing.h
	gcc -c Windowing.c
	
Buffer.o: Buffer.c Buffer.h
	gcc -c Buffer.c
	
clean:
	rm *.o Windowing
