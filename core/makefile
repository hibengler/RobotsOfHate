#system: test_superpos superpos.a 
system: test_superpos test2_draw_superpos

CCFLAGS= -g 

test_superpos: test_superpos.o superpos.o
	gcc $(CCFLAGS)  -o test_superpos test_superpos.o superpos.o -lm
	
superpos.o: superpos.c superpos.h
	gcc $(CCFLAGS) -c superpos.c

test_superpos.o: test_superpos.c superpos.h	
	gcc $(CCFLAGS) -c test_superpos.c
	
hibfonts.a: letters.c letters.h letters_render_opengles2.c letters_render_opengles2.h letters_ascii.c letters_ascii.h	 
	gcc $(CCFLAGS)  -c letters.c letters_render_opengles2.c letters_ascii.c
	ar rcs hibfonts.a  letters.o letters_render_opengles2.o letters_ascii.o

superposition: superposition.c
	gcc -o superposition -g superposition.c

CFLAGS = -g 
LDFLAGS = 

test2_draw_superpos: test2_draw_superpos.o superpos.o hibfonts.a
	 gcc $(LDFLAGS)  -g -o test2_draw_superpos test2_draw_superpos.o superpos.o hibfonts.a   -lm  -lGLESv2 -lglfw
	 

test2_draw_superpos.o: test2_draw_superpos.c superpos.h	
	gcc $(CCFLAGS) -c -g test2_draw_superpos.c
