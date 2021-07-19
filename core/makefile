#system: test_superpos superpos.a 
system: test_superpos 

CCFLAGS= -g 

test_superpos: test_superpos.o superpos.o
	gcc $(CCFLAGS)  -o test_superpos test_superpos.o superpos.o -lm
	
superpos.o: superpos.c superpos.h
	gcc $(CCFLAGS) -c superpos.c

test_superpos.o: test_superpos.c superpos.h	
	gcc $(CCFLAGS) -c test_superpos.c
	

superposition: superposition.c
	gcc -o superposition -g superposition.c
