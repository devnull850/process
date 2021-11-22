
test: test.o
	ld -o test test.o

test.o: test.s
	as -o test.o test.s

rip: rip.c
	gcc -Wall rip.c -o rip

process: process.c
	gcc -Wall process.c -o process

nop: nop.c
	gcc -Wall nop.c -o nop

clean:
	rm test test.o rip nop process blob
