all:pi.c
	gcc pi.c -o pi.out -lm -pthread
clean:
	rm pi.out
