CFLAGS = -g -p -pthread
LDFLAGS =  -lpthread



#system: test_superpos superpos.a 
system: test_superpos test2_draw_superpos test3_draw_robots test4_with_sound test5_player test6_playern2 test7_player2


test_superpos: test_superpos.o superpos.o
	gcc $(CFLAGS)  -o test_superpos test_superpos.o superpos.o -lm
	
superpos.o: superpos.c superpos.h
	gcc $(CFLAGS) -c superpos.c

test_superpos.o: test_superpos.c superpos.h	
	gcc $(CFLAGS) -c test_superpos.c
	
hibfonts.a: letters.c letters.h letters_render_opengles2.c letters_render_opengles2.h letters_ascii.c letters_ascii.h	 
	gcc $(CFLAGS)  -c letters.c letters_render_opengles2.c letters_ascii.c
	ar rcs hibfonts.a  letters.o letters_render_opengles2.o letters_ascii.o

superposition: superposition.c
	gcc -o superposition -g superposition.c

test2_draw_superpos: test2_draw_superpos.o superpos.o hibfonts.a helper.o
	 gcc $(LDFLAGS)  -g -o test2_draw_superpos test2_draw_superpos.o helper.o superpos.o hibfonts.a   -lm  -lGLESv2 -lglfw
	 

test2_draw_superpos.o: test2_draw_superpos.c superpos.h	
	gcc $(CFLAGS) -c -g test2_draw_superpos.c

test3_draw_robots:  superpos.o hibfonts.a larry_harvey_has_a_possee.o player.o \
	  onecolor_specs.o test3_draw_robots.o helper.o
	 gcc $(LDFLAGS)  -g -o test3_draw_robots test3_draw_robots.o superpos.o  \
	 	onecolor_specs.o  helper.o \
	     larry_harvey_has_a_possee.o player.o hibfonts.a   -lm  -lGLESv2 -lglfw

test3_draw_robots.o: test3_draw_robots.c superpos.h	
	gcc $(CFLAGS) -c -g test3_draw_robots.c

helper.o: helper.c helper.h
	gcc -c -g helper.c




onecolor_specs,o: onecolor_specs.c onecolor_specs.h
	gcc -o onecolor_specs -g onecolor_specs.c

multicolor_specs,o: multicolor_specs.c multicolor_specs.h
	gcc -o multicolor_specs -g multicolor_specs.c

test4_with_sound:  superpos.o hibfonts.a larry_harvey_has_a_possee.o player.o \
	  onecolor_specs.o test3_draw_robots.o helper.o \
	   simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
         simple_seven_eight_decode.o simple_sound_alsa.o simple_packet_summary.o \
	  \
       		  test4_with_sound.o
	 gcc $(LDFLAGS)  -g \
	  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR -L/usr/local/lib \
	    /usr/lib/x86_64-linux-gnu/libcurl.so -lasound -lopus -lraylib -lGL -lpthread -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread \
	 -o test4_with_sound test4_with_sound.o superpos.o  \
	 simple_curl_web_stream.o simple_decrypt.o simple_processor.o \
	 simple_seven_eight_decode.o simple_sound_alsa.o simple_packet_summary.o \
	 	onecolor_specs.o  helper.o \
	     larry_harvey_has_a_possee.o player.o hibfonts.a   -lm  -lGLESv2 -lglfw \
	       /usr/lib/x86_64-linux-gnu/libcurl.so -lasound -lopus  -lGL -lpthread \
	       -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread
	       

test6_playern2: network2.o test6_playern2.o network1.o 
	gcc -p $(LDFLAGS)  -g -o test6_playern2 test6_playern2.o network1.o  network2.o  -lpthread
	

simple.a: helper.o simple_curl_web_stream.o simple_decrypt.o simple_processor.o simple_packet_summary.o simple_sound_alsa.o simple_seven_eight_decode.o
	ar -rcs simple.a helper.o simple_curl_web_stream.o simple_decrypt.o simple_processor.o simple_packet_summary.o simple_sound_alsa.o simple_seven_eight_decode.o 	

test7_player2: simple.a video_planets.o \
        larry_harvey_has_a_possee.o player2.o \
	  onecolor_specs.o  multicolor_specs.o \
          test7_player2.o network2.o network1.o hibfonts.a test7_player2.o network1.o
	 gcc $(LDFLAGS)  -g \
	  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR -L/usr/local/lib \
	    /usr/lib/x86_64-linux-gnu/libcurl.so -lasound -lopus -lraylib -lGL -lpthread -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread \
	   -o test7_player2 test7_player2.o simple.a video_planets.o \
	 	onecolor_specs.o network1.o network2.o multicolor_specs.o \
	     larry_harvey_has_a_possee.o player2.o hibfonts.a   -lm  -lGLESv2 -lglfw \
	       /usr/lib/x86_64-linux-gnu/libcurl.so -lasound -lopus  -lGL -lpthread \
	       -ldl -lrt -lX11 -lm -DPLATFORM_DESKTOP -pthread
		
	
test5_player: test5_player.o network1.o 
	gcc -p $(LDFLAGS)  -g -o test5_player test5_player.o network1.o   -lpthread

test4_with_sound.o: test4_with_sound.c superpos.h	
	gcc $(CFLAGS) -c -g test4_with_sound.c

simple_sound_raylib.o: simple_sound_raylib.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR -DPLATFORM_DESKTOP -D_DEFAULT_SOURCE  simple_sound_raylib.c

simple_curl_web_stream.o: simple_curl_web_stream.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  simple_curl_web_stream.c

simple_decrypt.o: simple_decrypt.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  simple_decrypt.c

simple_processor.o: simple_processor.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  simple_processor.c

simple_seven_eight_decode.o: simple_seven_eight_decode.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  simple_seven_eight_decode.c

simple_sound_alsa.o: simple_sound_alsa.c
	gcc -c  $(CFLAGS) -Wl,-rpath -Wl,LIBDIR  simple_sound_alsa.c


simple_packet_summary.o: simple_packet_summary.c
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  simple_packet_summary.c


test5_player.o: test5_player.c network1.h
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  test5_player.c

test6_playern2.o: test6_playern2.c network1.h network2.h
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  test6_playern2.c

network1.o: network1.c network1.h
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  network1.c

network2.o: network2.c network2.h network1.h
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  network2.c

player2.o: player2.c
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  player2.c


test7_player2.o: test7_player2.c
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  test7_player2.c

video_planets.o: video_planets.c video_planets.h
	gcc -c  $(CFLAGS)  -Wl,-rpath -Wl,LIBDIR  video_planets.c

