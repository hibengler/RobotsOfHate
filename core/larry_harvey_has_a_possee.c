
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* this is different than the git version and tailored somewhat to silent radiance */
#define LARRY_GLES_2_DIRECT 1

#ifdef LARRY_GLES_2_DIRECT
#include "larry_gles2_shim.h"
#else
//#include "raylib.h"
//#include "rlgl.h"

#endif
#include "not_rl.h"
#include "larry_harvey_has_a_possee.h"




#define CLITERAL NOT_RL_CLITERAL






/* EVIL EVIL EVIL EVIL EVIL EVIL EVIL team (dark red) EVIL EVIL EVIL EVIL EVIL EVIL EVIL */
/* vvvvvvvvvv                                                                vvvvvvvvvvv */

void init_larry_harvey_evil_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{149,11,24,255};   	// Dark red
probot->usual_second_color = CLITERAL{205,130,91,255};     	// Tan Chaps
probot->usual_third_color  = CLITERAL{0,0,0,0};       		// No third color
probot->number_third_color_sections = 0;			// Only 2 groups has 3 colors.
probot->third_sections = NULL;
}




/*
        XX    
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_evil_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_evil_standard(probot);
probot->number_first_color_sections = 6;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-3,.y=-15,.width=6,.height=6};  // top noogie
probot->first_sections[4].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;
}


/*  
    XX      XX
      XX  XX  
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
      XX  XX  
    XX      XX
*/	 
void init_larry_harvey_evil_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_evil_standard(probot);
probot->number_first_color_sections = 9;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};  // left ear
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};  // right ear
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=-21,.width=6,.height=6}; // left antennae
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=-21,.width=6,.height=6};   // right antennae
probot->first_sections[5].the_section = (Rectangle) {.x=-9,.y=9,.width=6,.height=6};   // left foot
probot->first_sections[6].the_section = (Rectangle) {.x=3,.y=9,.width=6,.height=6};   	// right foot
probot->first_sections[7].the_section = (Rectangle) {.x=-15,.y=15,.width=6,.height=6};  // left pointantennae
probot->first_sections[8].the_section = (Rectangle) {.x=9,.y=15,.width=6,.height=6};   	// right pointantennae

probot->number_second_color_sections = 0;
probot->second_sections = NULL;
}



/*  
        XX  
    XX  XX  XX  
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
          
*/	 
void init_larry_harvey_evil_center(larry_harvey_robot *probot) {
init_larry_harvey_evil_standard(probot);
probot->number_first_color_sections = 8;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-15,.y=-15,.width=6,.height=6}; // left ear
probot->first_sections[2].the_section = (Rectangle) {.x=9,.y=-15,.width=6,.height=6};   // right ear
probot->first_sections[3].the_section = (Rectangle) {.x=-3,.y=-21,.width=6,.height=12};  // top horn
probot->first_sections[4].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[5].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm
probot->first_sections[6].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[7].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;
}






/*  
    XX      XX  
    XXXX  XXXX  
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
    XXXX  XXXX
    XX      XX
*/	 
void init_larry_harvey_evil_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_evil_standard(probot);
probot->number_first_color_sections = 9;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-15,.y=-21,.width=6,.height=12}; // left ear
probot->first_sections[2].the_section = (Rectangle) {.x=9,.y=-21,.width=6,.height=12};   // right ear
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};  // left ear2
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};   // right ear2
probot->first_sections[5].the_section = (Rectangle) {.x=-15,.y=9,.width=6,.height=12}; // left foot
probot->first_sections[6].the_section = (Rectangle) {.x=9,.y=9,.width=6,.height=12};   // right foot
probot->first_sections[7].the_section = (Rectangle) {.x=-9,.y=9,.width=6,.height=6};   // left foot2
probot->first_sections[8].the_section = (Rectangle) {.x=3,.y=9,.width=6,.height=6};   	// right foot2

probot->number_second_color_sections = 0;
probot->second_sections = NULL;
}


/*  


    XXXXXXXXXX   
  XXXX  XX  XXXX
  XXXXXXXXXXXXXX
    XX222222XX
    XX22  22XX
*/	 
void init_larry_harvey_evil_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_evil_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=12}; // left ear
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=12};  // right ear
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=9,.width=6,.height=12}; // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=9,.width=6,.height=12};   // right foot


probot->number_second_color_sections = 3;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-9,.y=9,.width=18,.height=6};  // bottom pants bar
probot->second_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};  // left pants leg
probot->second_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   // right pants leg
}


/* EVIL EVIL EVIL EVIL EVIL EVIL EVIL team (dark red) EVIL EVIL EVIL EVIL EVIL EVIL EVIL

   point guard      shooting guard      center        small forward  power forward 
                      XX      XX          XX            XX      XX  
        XX              XX  XX        XX  XX  XX        XXXX  XXXX  
    XXXXXXXXXX        XXXXXXXXXX      XXXXXXXXXX        XXXXXXXXXX      XXXXXXXXXX       
  XXXX  XX  XXXX      XX  XX  XX    XXXX  XX  XXXX      XX  XX  XX    XXXX  XX  XXXX
    XXXXXXXXXX        XXXXXXXXXX      XXXXXXXXXX        XXXXXXXXXX    XXXXXXXXXXXXXX
    XXXXXXXXXX          XX  XX        XXXXXXXXXX        XXXX  XXXX      XX222222XX
      XX  XX          XX      XX        XX  XX          XX      XX      XX22  22XX

   EVIL EVIL EVIL EVIL EVIL EVIL EVIL team (dark red) EVIL EVIL EVIL EVIL EVIL EVIL EVIL */
void init_larry_harvey_evil(larry_harvey_basketball_team *pteam) {
init_larry_harvey_evil_point_guard(pteam->players+0);
init_larry_harvey_evil_shooting_guard(pteam->players+1);
init_larry_harvey_evil_center(pteam->players+2);
init_larry_harvey_evil_small_forward(pteam->players+3);
init_larry_harvey_evil_power_forward(pteam->players+4);
}





















/* LOVE LOVE LOVE LOVE LOVE LOVE LOVE team (light red/pink) LOVE LOVE LOVE LOVE LOVE LOVE LOVE */



                
void init_larry_harvey_love_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{253,34,38,255};   	// Redder almost pink
probot->usual_second_color = CLITERAL{0,0,0,0};         	// No second color
probot->usual_third_color  = CLITERAL{0,0,0,0};       		// No third color
probot->number_second_color_sections = 0;			// No second color for love
probot->second_sections = NULL;
probot->number_third_color_sections = 0;			// Only 2 groups has 3 colors.
probot->third_sections = NULL;
}




/*
      XX  XX
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_love_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_love_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};  // left nub
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};   // right nub
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot
}



/*
        
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
    XX      XX  
*/	 
void init_larry_harvey_love_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_love_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// Main chunk
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};   // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=15,.width=6,.height=6};  // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=15,.width=6,.height=6};   	// right foot

}




/*
    XX      XX
      XX  XX      
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_love_center(larry_harvey_robot *probot) {
init_larry_harvey_love_standard(probot);
probot->number_first_color_sections = 9;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};  // left nub
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};   // right nub
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=-21,.width=6,.height=6}; // left antannae
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=-21,.width=6,.height=6};   // right antannae
probot->first_sections[5].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[6].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};   // right arm
probot->first_sections[7].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[8].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

}



/*

      XX  XX      
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
  XXXXXXXXXXXXXX
  XXXX      XXXX
*/	 
void init_larry_harvey_love_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_love_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};  // left nub
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};   // right nub
probot->first_sections[3].the_section = (Rectangle) {.x=-21,.y=15,.width=12,.height=6}; // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=15,.width=12,.height=6};   // right foot
probot->first_sections[5].the_section = (Rectangle) {.x=-21,.y=9,.width=6,.height=6};   // left foot2
probot->first_sections[6].the_section = (Rectangle) {.x=15,.y=9,.width=6,.height=6};    // right foot2

}


/*

      XX  XX      
    XXXXXXXXXX   
XXXXXX  XX  XXXXXX
  XXXXXXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_love_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_love_standard(probot);
probot->number_first_color_sections = 9;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24}; // Main chunk
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};   // left nub
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};    // right nub
probot->first_sections[3].the_section = (Rectangle) {.x=-27,.y=-3,.width=12,.height=6};  // left arm
probot->first_sections[4].the_section = (Rectangle) {.x=15,.y=-3,.width=12,.height=6};   // right arm
probot->first_sections[5].the_section = (Rectangle) {.x=-21,.y=3,.width=6,.height=6};    // left arm2
probot->first_sections[6].the_section = (Rectangle) {.x=15,.y=3,.width=6,.height=6};     // right arm2
probot->first_sections[7].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};    // left foot
probot->first_sections[8].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};     // right foot

}
		





/* LOVE LOVE LOVE LOVE LOVE LOVE LOVE team (light red/pink) LOVE LOVE LOVE LOVE LOVE LOVE LOVE 
                                    XX      XX  
      XX  XX                          XX  XX          XX  XX            XX  XX      
    XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX        XXXXXXXXXX
    XX  XX  XX    XXXX  XX  XXXX  XXXX  XX  XXXX    XX  XX  XX    XXXXXX  XX  XXXXXX
    XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXXXXXX
    XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX    XXXXXXXXXXXXXX      XXXXXXXXXX
      XX  XX        XX      XX        XX  XX      XXXX      XXXX        XX  XX


   LOVE LOVE LOVE LOVE LOVE LOVE LOVE team (light red/pink) LOVE LOVE LOVE LOVE LOVE LOVE LOVE */

void init_larry_harvey_love(larry_harvey_basketball_team *pteam) {
init_larry_harvey_love_point_guard(pteam->players+0);
init_larry_harvey_love_shooting_guard(pteam->players+1);
init_larry_harvey_love_center(pteam->players+2);
init_larry_harvey_love_small_forward(pteam->players+3);
init_larry_harvey_love_power_forward(pteam->players+4);
}












/* BURNER BURNER BURNER BURNER BURNER BURNER BURNER team (light orange) BURNER BURNER BURNER BURNER BURNER BURNER BURNER */

void init_larry_harvey_burner_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{255,147,25,255};   	// Orange fire
probot->usual_second_color = CLITERAL{0,0,0,0};         	// No second color
probot->usual_third_color  = CLITERAL{0,0,0,0};       		// No third color
probot->number_second_color_sections = 0;			// No second color for burner
probot->second_sections = NULL;
probot->number_third_color_sections = 0;			// Only 2 groups has 3 colors.
probot->third_sections = NULL;
}



/*
      XXXXXX
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
    XXXXXXXXXX
    XX  XX  XX  
*/	 
void init_larry_harvey_burner_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_burner_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// base
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // noggin
probot->first_sections[2].the_section = (Rectangle) {.x=-15,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[3].the_section = (Rectangle) {.x=9,.y=15,.width=6,.height=6};   	// right foot
probot->first_sections[4].the_section = (Rectangle) {.x=-3,.y=15,.width=6,.height=6};  	// uh middle foot?
}



/*  
        XX  
    XX  XX  XX  
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
      XX  XX  
      XX  XX  
          
*/
void init_larry_harvey_burner_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_burner_standard(probot);
probot->number_first_color_sections = 6;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-15,.y=-15,.width=6,.height=6}; // left ear
probot->first_sections[2].the_section = (Rectangle) {.x=9,.y=-15,.width=6,.height=6};   // right ear
probot->first_sections[3].the_section = (Rectangle) {.x=-3,.y=-21,.width=6,.height=12};  // top horn
probot->first_sections[4].the_section = (Rectangle) {.x=-9,.y=9,.width=6,.height=12};   // left foot
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=9,.width=6,.height=12};   	// right foot

}


/*


    XXXXXXXXXX   
XXXXXX  XX  XXXXXX
  XXXXXXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_burner_center(larry_harvey_robot *probot) {
init_larry_harvey_burner_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24}; // Main chunk
probot->first_sections[1].the_section = (Rectangle) {.x=-27,.y=-3,.width=12,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=12,.height=6};   // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-21,.y=3,.width=6,.height=6};    // left arm2
probot->first_sections[4].the_section = (Rectangle) {.x=15,.y=3,.width=6,.height=6};     // right arm2
probot->first_sections[5].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};    // left foot
probot->first_sections[6].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};     // right foot

}



/*
        XX    
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXX  XXXX
      XX  XX  
*/	 
void init_larry_harvey_burner_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_burner_standard(probot);
probot->number_first_color_sections = 8;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-3,.y=-15,.width=6,.height=6};  // top noogie
probot->first_sections[4].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot
probot->first_sections[6].the_section = (Rectangle) {.x=-15,.y=9,.width=12,.height=6};   // left foot2
probot->first_sections[7].the_section = (Rectangle) {.x=3,.y=9,.width=12,.height=6};   	// right foot2

}


/*  
        XX      
      XXXXXX  
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
      XX  XX  
    XX      XX
*/	 
void init_larry_harvey_burner_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_burner_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // nub 1
probot->first_sections[2].the_section = (Rectangle) {.x=-3,.y=-21,.width=6,.height=6};  // nub 2
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=9,.width=6,.height=6};   // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=9,.width=6,.height=6};   	// right foot
probot->first_sections[5].the_section = (Rectangle) {.x=-15,.y=15,.width=6,.height=6};  // left pointantennae
probot->first_sections[6].the_section = (Rectangle) {.x=9,.y=15,.width=6,.height=6};   	// right pointantennae

}


/* BURNER BURNER BURNER BURNER BURNER BURNER BURNER team (light orange) BURNER BURNER BURNER BURNER BURNER BURNER BURNER 
                      XX                                                XX
      XXXXXX      XX  XX  XX                            XX            XXXXXX
    XXXXXXXXXX    XXXXXXXXXX      XXXXXXXXXX        XXXXXXXXXX      XXXXXXXXXX
    XX  XX  XX    XX  XX  XX  XXXXXX  XX  XXXXXX  XXXX  XX  XXXX    XX  XX  XX
    XXXXXXXXXX    XXXXXXXXXX    XXXXXXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX
    XXXXXXXXXX      XX  XX        XXXXXXXXXX        XXXX  XXXX        XX  XX
    XX  XX  XX      XX  XX          XX  XX            XX  XX        XX      XX
   BURNER BURNER BURNER BURNER BURNER BURNER BURNER team (light orange) BURNER BURNER BURNER BURNER BURNER BURNER BURNER */

void init_larry_harvey_burner(larry_harvey_basketball_team *pteam) {
init_larry_harvey_burner_point_guard(pteam->players+0);
init_larry_harvey_burner_shooting_guard(pteam->players+1);
init_larry_harvey_burner_center(pteam->players+2);
init_larry_harvey_burner_small_forward(pteam->players+3);
init_larry_harvey_burner_power_forward(pteam->players+4);
}















/* STAR STAR STAR STAR STAR STAR STAR team (yellow) STAR STAR STAR STAR STAR STAR STAR */

void init_larry_harvey_star_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{255,251,19,255};   	// yellow
probot->usual_second_color = CLITERAL{179,144,106,255};         // Tan pants
probot->usual_third_color  = CLITERAL{0,0,0,0};       		// No third color

probot->number_third_color_sections = 0;			// Only 2 teams has 3 colors.
probot->third_sections = NULL;
}



/*
    XX      XX
      XXXXXX
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXXXXXX

*/	 
void init_larry_harvey_star_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_star_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// base
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // noggin
probot->first_sections[2].the_section = (Rectangle) {.x=-15,.y=-21,.width=6,.height=6};  // left earthing
probot->first_sections[3].the_section = (Rectangle) {.x=9,.y=-21,.width=6,.height=6};    // right earthing
probot->first_sections[4].the_section = (Rectangle) {.x=15,.y=9,.width=12,.height=6}; 	// one foot
probot->first_sections[5].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[6].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm

probot->number_second_color_sections = 0;                       
probot->second_sections = NULL;
}


/*


    XXXXXXXXXX   
XXXXXX  XX  XXXXXX
  XXXXXXXXXXXXXX
    XX222222XX
    XX22  22XX
*/	 
void init_larry_harvey_star_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_star_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18}; // Main chunk
probot->first_sections[1].the_section = (Rectangle) {.x=-27,.y=-3,.width=12,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=12,.height=6};   // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=-3,.width=6,.height=6};  // left arm2
probot->first_sections[4].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};   // right arm2
probot->first_sections[5].the_section = (Rectangle) {.x=-15,.y=9,.width=6,.height=12};    // left foot
probot->first_sections[6].the_section = (Rectangle) {.x=9,.y=9,.width=6,.height=12};     // right foot

probot->number_second_color_sections = 3;   
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-9,.y=9,.width=18,.height=6};  // bottom pants bar
probot->second_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};  // left pants leg
probot->second_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   // right pants leg
}





/*
          XX
      XX^^^^    
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_star_center(larry_harvey_robot *probot) {
init_larry_harvey_star_standard(probot);
probot->number_first_color_sections = 8;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};  // top noogie
probot->first_sections[4].the_section = (Rectangle) {.x=-3,.y=-15,.width=12,.height=5};  // top noogie2
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=-21,.width=6,.height=6};  // top noogie3
probot->first_sections[6].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[7].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;                       
probot->second_sections = NULL;
}



/*
      
  XXXX      XXXX
  XXXXXXXXXXXXXX
    XX  XX  XX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_star_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_star_standard(probot);

probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-9,.width=6,.height=6};  // left ear2
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-9,.width=6,.height=6};  // right ear2
probot->first_sections[3].the_section = (Rectangle) {.x=-21,.y=-15,.width=12,.height=6};  // top ear
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=-15,.width=12,.height=6};  // top ear
probot->first_sections[5].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[6].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;                       
probot->second_sections = NULL;
}


/*
    XX      XX
    XXXX  XXXX    
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_star_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_star_standard(probot);
probot->number_first_color_sections = 9;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=-15,.width=12,.height=6};  // left ear1
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=-15,.width=12,.height=6};  // right ear1
probot->first_sections[5].the_section = (Rectangle) {.x=-15,.y=-15,.width=6,.height=6};  // left ear2
probot->first_sections[6].the_section = (Rectangle) {.x=9,.y=-15,.width=6,.height=6};  // right ear2

probot->first_sections[7].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[8].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;                       
probot->second_sections = NULL;
}





/* STAR STAR STAR STAR STAR STAR STAR team (yellow) STAR STAR STAR STAR STAR STAR STAR 

    XX      XX                                XX                        XX      XX
      XXXXXX                              XX^^^^      XXXX      XXXX    XXXX  XXXX
    XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX    XXXXXXXXXXXXXX    XXXXXXXXXX
  XXXX  XX  XXXX  XXXXXX  XX  XXXXXX  XXXX  XX  XXXX    XX  XX  XX    XXXX  XX  XXXX
    XXXXXXXXXX      XXXXXXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX
    XXXXXXXXXXXXXX    XX222222XX        XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX
                      XX22  22XX          XX  XX          XX  XX          XX  XX


   STAR STAR STAR STAR STAR STAR STAR team (yellow) STAR STAR STAR STAR STAR STAR STAR */
void init_larry_harvey_star(larry_harvey_basketball_team *pteam) {
init_larry_harvey_star_point_guard(pteam->players+0);
init_larry_harvey_star_shooting_guard(pteam->players+1);
init_larry_harvey_star_center(pteam->players+2);
init_larry_harvey_star_small_forward(pteam->players+3);
init_larry_harvey_star_power_forward(pteam->players+4);
}











/* SCREW SCREW SCREW SCREW SCREW SCREW SCREW team (yellow) SCREW SCREW SCREW SCREW SCREW SCREW SCREW */
void init_larry_harvey_screw_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{27,192,87,255};   	// Green
probot->usual_second_color = CLITERAL{195,244,31,255};         //  pale green hair
probot->usual_third_color  = CLITERAL{0,0,0,0};       		// No third color

probot->number_third_color_sections = 0;			// Only 2 teams has 3 colors.
probot->third_sections = NULL;
}


/*
        2222
    22222222
  2222222222      
2222XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_screw_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_screw_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 4;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-15,.y=-21,.width=24,.height=12}; // hair bulk
probot->second_sections[1].the_section = (Rectangle) {.x=-21,.y=-15,.width=6,.height=6};   // hair bit
probot->second_sections[2].the_section = (Rectangle) {.x=-27,.y=-9,.width=12,.height=6};   // hair left
probot->second_sections[3].the_section = (Rectangle) {.x=-3,.y=-27,.width=12,.height=6};   // hair top

}

/*

    2222222222
  2222  22  2222
    2222222222
    XXXXXXXXXX
      XX  XX  
 */
void init_larry_harvey_screw_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_screw_standard(probot);
probot->number_first_color_sections = 3;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=9,.width=30,.height=6};// Main chunk
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};  // left foot
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot


probot->number_second_color_sections = 3;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->second_sections[1].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};   // right arm
probot->second_sections[2].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18}; // light face
}




/*
      222222
      222222222222
      222222222222      
    XXXXXXXXXX2222   
    XX  XX  XX22
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/
void init_larry_harvey_screw_center(larry_harvey_robot *probot) {
init_larry_harvey_screw_standard(probot);
probot->number_first_color_sections = 3;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 4;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-9,.y=-21,.width=36,.height=12}; // hair bulk
probot->second_sections[1].the_section = (Rectangle) {.x=15,.y=-9,.width=12,.height=6};   // hair bit
probot->second_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};   // hair more
probot->second_sections[3].the_section = (Rectangle) {.x=-9,.y=-27,.width=18,.height=6};   // hair top
}



/*
        22
      222222
    2222222222
  XXXXXXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_screw_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_screw_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=-9,.width=6,.height=6};   // left shoulder pad
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right shoulder pad

probot->number_second_color_sections = 3;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-15,.y=-15,.width=30,.height=6}; // tower bottom
probot->second_sections[1].the_section = (Rectangle) {.x=-9,.y=-21,.width=18,.height=6};   // tower mid
probot->second_sections[2].the_section = (Rectangle) {.x=-3,.y=-27,.width=6,.height=6};   // tower top

}



/*
        22
    2222222222
  2222  22  2222
    2222222222
    XXXXXXXXXX
    XXXXXXXXXX
    XX      XX  
 */
void init_larry_harvey_screw_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_screw_standard(probot);
probot->number_first_color_sections = 3;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=9,.width=30,.height=12};// Main chunk
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=21,.width=6,.height=6};  // left foot
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=21,.width=6,.height=6};   	// right foot


probot->number_second_color_sections = 4;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};  // top pale green
probot->second_sections[1].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};   // right arm
probot->second_sections[2].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6}; // left arm
probot->second_sections[3].the_section = (Rectangle) {.x=-3,.y=-15,.width=6,.height=6}; // top nougie
}







/* SCREW SCREW SCREW SCREW SCREW SCREW SCREW team (green) SCREW SCREW SCREW SCREW SCREW SCREW SCREW 
        2222                        222222               22        
    22222222                        222222222222       222222
  2222222222                        222222222222     2222222222          22
2222XXXXXXXXXX      2222222222    XXXXXXXXXX2222   XXXXXXXXXXXXXX    2222222222
  XXXX  XX  XXXX  2222  22  2222  XX  XX  XX2        XX  XX  XX    2222  22  2222
    XXXXXXXXXX      2222222222    XXXXXXXXXX         XXXXXXXXXX      2222222222
    XXXXXXXXXX      XXXXXXXXXX    XXXXXXXXXX         XXXXXXXXXX      XXXXXXXXXX
      XX  XX          XX  XX        XX  XX             XX  XX        XX      XX

   SCREW SCREW SCREW SCREW SCREW SCREW SCREW team (green) SCREW SCREW SCREW SCREW SCREW SCREW SCREW */

void init_larry_harvey_screw(larry_harvey_basketball_team *pteam) {
init_larry_harvey_screw_point_guard(pteam->players+0);
init_larry_harvey_screw_shooting_guard(pteam->players+1);
init_larry_harvey_screw_center(pteam->players+2);
init_larry_harvey_screw_small_forward(pteam->players+3);
init_larry_harvey_screw_power_forward(pteam->players+4);
}













/* PASTA PASTA PASTA PASTA PASTA PASTA PASTA team (lightblue) PASTA PASTA PASTA PASTA PASTA PASTA PASTA */

void init_larry_harvey_pasta_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{20,183,248,255};   	// light blue
probot->usual_second_color = CLITERAL{201,135,113,255};         // pants
probot->usual_third_color  = CLITERAL{255,255,255,255};       		// There is a third color - white.

}





/*
    XX      XX
    XXXXXXXXXX   
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 

void init_larry_harvey_pasta_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_pasta_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left arm
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right arm
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=-15,.width=6,.height=6};  // left noogie
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=-15,.width=6,.height=6};  // right noogie
probot->first_sections[5].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[6].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;
probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}




/*
  XX          XX
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_pasta_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_pasta_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-15,.width=6,.height=6};  // left pigtail
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-15,.width=6,.height=6};  // right pigtail
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;
probot->number_third_color_sections = 0;
probot->third_sections = NULL;

}



/*
      XX  XX
    XXXXXXXXXX   
  XXXX  XX  XXXX
  XXXXXXXXXXXXXX
  XXXXXX  XXXXXX  
*/	 
void init_larry_harvey_pasta_center(larry_harvey_robot *probot) {
init_larry_harvey_pasta_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};  // left horn
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};  // right horn
probot->first_sections[3].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=18};   // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=18};   	// right foot
probot->first_sections[5].the_section = (Rectangle) {.x=-15,.y=9,.width=12,.height=6};   // left foot2
probot->first_sections[6].the_section = (Rectangle) {.x=3,.y=9,.width=12,.height=6};   	// right foot2

probot->number_second_color_sections = 0;
probot->second_sections = NULL;
probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}

/*      33
      333333
  XXXXXXXXXXXXXX   
  33XX  XX  XX33
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/
void init_larry_harvey_pasta_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_pasta_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-9,.width=6,.height=6};  // left shoulder
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-9,.width=6,.height=6};  // right shoulder
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;

probot->number_third_color_sections = 4;
probot->third_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_third_color_sections);;
probot->third_sections[0].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=6};  // left shoulder glove
probot->third_sections[1].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=6};  // right shoulder glove
probot->third_sections[2].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // bottom hat
probot->third_sections[3].the_section = (Rectangle) {.x=-3,.y=-21,.width=6,.height=6};  // top hat

}



/*
    XX      XX
    XXXXXXXXXX   
  XXXX  XX  XXXX
  XXXXXXXXXXXXXX
    XX222222XX
    XX22  22XX  
*/	 
void init_larry_harvey_pasta_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_pasta_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-3,.width=6,.height=12}; // left ear
probot->first_sections[2].the_section = (Rectangle) {.x=15,.y=-3,.width=6,.height=12};  // right ear
probot->first_sections[3].the_section = (Rectangle) {.x=-15,.y=9,.width=6,.height=12}; // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=9,.y=9,.width=6,.height=12};   // right foot
probot->first_sections[5].the_section = (Rectangle) {.x=9,.y=-15,.width=6,.height=6}; // left antanne
probot->first_sections[6].the_section = (Rectangle) {.x=-15,.y=-15,.width=6,.height=6};   // right antennae

probot->number_second_color_sections = 3;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-9,.y=9,.width=18,.height=6};  // bottom pants bar
probot->second_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};  // left pants leg
probot->second_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   // right pants leg

probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}


/* PASTA PASTA PASTA PASTA PASTA PASTA PASTA team (lightblue) PASTA PASTA PASTA PASTA PASTA PASTA PASTA 

                                                          33
    XX      XX    XX          XX        XX  XX          333333        XX      XX
    XXXXXXXXXX      XXXXXXXXXX        XXXXXXXXXX    XXXXXXXXXXXXXX    XXXXXXXXXX
  XXXX  XX  XXXX    XX  XX  XX      XXXX  XX  XXXX  33XX  XX  XX33  XXXX  XX  XXXX
    XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXXXXXX    XXXXXXXXXX    XXXXXXXXXXXXXX
    XXXXXXXXXX      XXXXXXXXXX      XXXXXX  XXXXXX    XXXXXXXXXX      XX222222XX
      XX  XX          XX  XX                            XX  XX        XX22  22XX

   PASTA PASTA PASTA PASTA PASTA PASTA PASTA team (lightblue) PASTA PASTA PASTA PASTA PASTA PASTA PASTA */

void init_larry_harvey_pasta(larry_harvey_basketball_team *pteam) {
init_larry_harvey_pasta_point_guard(pteam->players+0);
init_larry_harvey_pasta_shooting_guard(pteam->players+1);
init_larry_harvey_pasta_center(pteam->players+2);
init_larry_harvey_pasta_small_forward(pteam->players+3);
init_larry_harvey_pasta_power_forward(pteam->players+4);
}




















/* HOPE HOPE HOPE HOPE HOPE HOPE HOPE team (dark blue) HOPE HOPE HOPE HOPE HOPE HOPE HOPE */

void init_larry_harvey_hope_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{51,80,210,255};   	// Blue
probot->usual_second_color = CLITERAL{0,0,0,0};         	// No second color
probot->usual_third_color  = CLITERAL{0,0,0,0};       		// No third color
probot->number_second_color_sections = 0;			// No second color for hope
probot->second_sections = NULL;
probot->number_third_color_sections = 0;			// Only 2 teams has 3 colors.
probot->third_sections = NULL;
}


/*
      XX
      XXXX
      XXXXXX
    XXXXXXXXXX   
    XX  XX  XX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_hope_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_hope_standard(probot);
probot->number_first_color_sections = 6;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // hairdew 1
probot->first_sections[2].the_section = (Rectangle) {.x=-9,.y=-21,.width=12,.height=6};  // hairdew 2
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=-27,.width=6,.height=6};   // hairdew 3
probot->first_sections[4].the_section = (Rectangle) {.x=-9,.y=9,.width=6,.height=6};     // left foot
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=9,.width=6,.height=6};   	 // right foot
}

/*

       XX  XX
     XXXX  XXXX
   XXXXXXXXXXXXXX
   XXXX  XX  XXXX
     XXXXXXXXXX
       XX  XX
*/   
void init_larry_harvey_hope_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_hope_standard(probot);
probot->number_first_color_sections = 8;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-21,.y=-9,.width=42,.height=12};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-15,.y=3,.width=30,.height=6};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[2].the_section = (Rectangle) {.x=-15,.y=-15,.width=12,.height=6};  // hairdew l1
probot->first_sections[3].the_section = (Rectangle) {.x=3,.y=-15,.width=12,.height=6};  // hairdew r1
probot->first_sections[4].the_section = (Rectangle) {.x=-9,.y=-21,.width=6,.height=6};  // hairdew l2
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=-21,.width=6,.height=6};  // hairdew r2
probot->first_sections[6].the_section = (Rectangle) {.x=-9,.y=9,.width=6,.height=6};     // left foot
probot->first_sections[7].the_section = (Rectangle) {.x=3,.y=9,.width=6,.height=6};   	 // right foot
}



/*

XXXXXX  XXXXXX
XXXXXXXXXXXXXX
  XX  XX  XX
  XXXXXXXXXX
  XXXXXXXXXX
    XX  XX
*/    

void init_larry_harvey_hope_center(larry_harvey_robot *probot) {
init_larry_harvey_hope_standard(probot);
probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// base
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};     // left foot
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	 // right foot
probot->first_sections[3].the_section = (Rectangle) {.x=-21,.y=-15,.width=6,.height=18};   // left mouseear
probot->first_sections[4].the_section = (Rectangle) {.x=15,.y=-15,.width=6,.height=18};   	// right mouseear
probot->first_sections[5].the_section = (Rectangle) {.x=-15,.y=-15,.width=12,.height=6};   // left mousear2
probot->first_sections[6].the_section = (Rectangle) {.x=3,.y=-15,.width=12,.height=6};  // right mouseear2
}




/*
    XX      XX
      XXXXXX   
    XX  XX  XX
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX  
*/	 
void init_larry_harvey_hope_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_hope_standard(probot);
probot->number_first_color_sections = 6;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // cranium
probot->first_sections[2].the_section = (Rectangle) {.x=-15,.y=-21,.width=6,.height=6};  // left whacker
probot->first_sections[3].the_section = (Rectangle) {.x=9,.y=-21,.width=6,.height=6};  // right whacker
probot->first_sections[4].the_section = (Rectangle) {.x=-9,.y=9,.width=6,.height=6};   // left foot
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=9,.width=6,.height=6};   	// right foot
}



/*

      XX  XX      
    XXXXXXXXXX   
XXXXXX  XX  XXXXXX
  XXXXXXXXXXXXXX
    XXXXXXXXXX
*/	 
void init_larry_harvey_hope_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_hope_standard(probot);
probot->number_first_color_sections =7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24}; // Main chunk
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=6,.height=6};   // left nub
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=-15,.width=6,.height=6};    // right nub
probot->first_sections[3].the_section = (Rectangle) {.x=-27,.y=-3,.width=12,.height=6};  // left arm
probot->first_sections[4].the_section = (Rectangle) {.x=15,.y=-3,.width=12,.height=6};   // right arm
probot->first_sections[5].the_section = (Rectangle) {.x=-21,.y=3,.width=6,.height=6};    // left arm2
probot->first_sections[6].the_section = (Rectangle) {.x=15,.y=3,.width=6,.height=6};     // right arm2

}




/* HOPE HOPE HOPE HOPE HOPE HOPE HOPE team (dark blue) HOPE HOPE HOPE HOPE HOPE HOPE HOPE 
      XX
      XXXX          XX  XX
      XXXXXX      XXXX  XXXX      XXXXXX  XXXXXX    XX      XX        XX  XX
    XXXXXXXXXX  XXXXXXXXXXXXXX    XXXXXXXXXXXXXX      XXXXXX        XXXXXXXXXX
    XX  XX  XX  XXXX  XX  XXXX      XX  XX  XX      XX  XX  XX  XXXXXX  XX  XXXXXX
    XXXXXXXXXX    XXXXXXXXXX        XXXXXXXXXX      XXXXXXXXXX    XXXXXXXXXXXXXX
      XX  XX        XX  XX          XXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX
                                      XX  XX          XX  XX
		    
   HOPE HOPE HOPE HOPE HOPE HOPE HOPE team (dark blue) HOPE HOPE HOPE HOPE HOPE HOPE HOPE */

void init_larry_harvey_hope(larry_harvey_basketball_team *pteam) {
init_larry_harvey_hope_point_guard(pteam->players+0);   
init_larry_harvey_hope_shooting_guard(pteam->players+1);
init_larry_harvey_hope_center(pteam->players+2);
init_larry_harvey_hope_small_forward(pteam->players+3);
init_larry_harvey_hope_power_forward(pteam->players+4);
}









/* GOA  GOA  GOA  GOA  GOA  GOA  GOA  team (deep purple blue) GOA  GOA  GOA  GOA  GOA  GOA  GOA  */

void init_larry_harvey_goa_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{151,0,198,255};   	// deep purple
probot->usual_second_color = CLITERAL{255,251,25,255};         // a blond hair
probot->usual_third_color  = CLITERAL{255,255,255,255};       		// There is a third color - white.
}


/*
    XXXXXX  
  XXXXXXXXXX
  XX  XX  XX
  XXXXXXXXXX
  XXXXXXXXXX
    XX  XX
*/
void init_larry_harvey_goa_point_guard(larry_harvey_robot *probot) {
init_larry_harvey_goa_standard(probot);
probot->number_first_color_sections = 4;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // top head
probot->first_sections[2].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[3].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;

probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}


/*
        XXXXXXXXXX
    XXXXXX  XX  XXXXXX
    XXXXXXXXXXXXXXXXXX
      3333333333XX33
      333333  33XX33
      XXXXXX  XXXXXX
*/
void init_larry_harvey_goa_shooting_guard(larry_harvey_robot *probot) {
init_larry_harvey_goa_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=6};// block1
probot->first_sections[1].the_section = (Rectangle) {.x=-27,.y=-3,.width=54,.height=12};// block2
probot->first_sections[2].the_section = (Rectangle) {.x=9,.y=9,.width=6,.height=12};   	// slip
probot->first_sections[3].the_section = (Rectangle) {.x=-21,.y=21,.width=18,.height=6};
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=21,.width=18,.height=6};

probot->number_second_color_sections = 0;
probot->second_sections = NULL;

probot->number_third_color_sections = 4;
probot->third_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_third_color_sections);;
probot->third_sections[0].the_section = (Rectangle) {.x=-21,.y=9,.width=30,.height=6};   // Wrap a round
probot->third_sections[1].the_section = (Rectangle) {.x=-21,.y=15,.width=18,.height=6};
probot->third_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};
probot->third_sections[3].the_section = (Rectangle) {.x=15,.y=9,.width=6,.height=12};
}

/*
      22
      2222
  XXXX222222XXXX
  XxXXXXXXXXXXxX
  33XX  XX  XX33
    XXXXXXXXXX
    XXXXXXXXXX
      XX  XX
*/
void init_larry_harvey_goa_center(larry_harvey_robot *probot) {
init_larry_harvey_goa_standard(probot);
probot->number_first_color_sections = 7; 
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 $
probot->first_sections[1].the_section = (Rectangle) {.x=-21,.y=-15,.width=12,.height=6};  // left pad top
probot->first_sections[2].the_section = (Rectangle) {.x=9,.y=-15,.width=12,.height=6};  // right pad top
probot->first_sections[3].the_section = (Rectangle) {.x=-21,.y=-9,.width=5,.height=6};  // left pad top
probot->first_sections[4].the_section = (Rectangle) {.x=16,.y=-9,.width=5,.height=6};  // right pad top
probot->first_sections[5].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[6].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};    // right foot

probot->number_second_color_sections = 3;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6}; // bottom hair
probot->second_sections[1].the_section = (Rectangle) {.x=-9,.y=-21,.width=12,.height=6}; // middle hair
probot->second_sections[2].the_section = (Rectangle) {.x=-9,.y=-27,.width=6,.height=6}; // top hair

probot->number_third_color_sections = 2;
probot->third_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_third_color_sections);;
probot->third_sections[0].the_section = (Rectangle) {.x=-21,.y=-3,.width=5,.height=6}; // left glove
probot->third_sections[1].the_section = (Rectangle) {.x=16,.y=-3,.width=5,.height=6}; // right glove
}

/*
  XXXXXXXXXX
  XX  XX  XX
  XXXXXXXXXX
    XX  XX  
*/    
void init_larry_harvey_goa_small_forward(larry_harvey_robot *probot) {
init_larry_harvey_goa_standard(probot);
probot->number_first_color_sections = 3;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=24};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=15,.width=6,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;

probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}

/*
    XXXXXX
  XXXXXXXXXX
  XX  XX  XX
  XXXXXXXXXX
  XXXX  XXXX
  XX      XX
*/
void init_larry_harvey_goa_power_forward(larry_harvey_robot *probot) {
init_larry_harvey_goa_standard(probot);
probot->number_first_color_sections = 6;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=-15,.width=18,.height=6};  // top head
probot->first_sections[2].the_section = (Rectangle) {.x=-15,.y=15,.width=6,.height=6};   // left foot
probot->first_sections[3].the_section = (Rectangle) {.x=9,.y=15,.width=6,.height=6};   	// right foot
probot->first_sections[4].the_section = (Rectangle) {.x=-15,.y=9,.width=12,.height=6};   // left foot
probot->first_sections[5].the_section = (Rectangle) {.x=3,.y=9,.width=12,.height=6};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;

probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}







/* GOA  GOA  GOA  GOA  GOA  GOA  GOA  team (deep purple blue) GOA  GOA  GOA  GOA  GOA  GOA  GOA  

                                        22
                                        2222
    XXXXXX                          XXXX222222XXXX                    XXXXXX
  XXXXXXXXXX      XXXXXXXXXX        XxXXXXXXXXXXxX    XXXXXXXXXX    XXXXXXXXXX
  XX  XX  XX  XXXXXX  XX  XXXXXX    33XX  XX  XX33    XX  XX  XX    XX  XX  XX
  XXXXXXXXXX  XXXXXXXXXXXXXXXXXX      XXXXXXXXXX      XXXXXXXXXX    XXXXXXXXXX
  XXXXXXXXXX    3333333333XX33        XXXXXXXXXX        XX  XX      XXXX  XXXX
    XX  XX      333333  33XX33          XX  XX                      XX      XX
                XXXXXX  XXXXXX


   GOA  GOA  GOA  GOA  GOA  GOA  GOA  team (deep purple blue) GOA  GOA  GOA  GOA  GOA  GOA  GOA  */


void init_larry_harvey_goa(larry_harvey_basketball_team *pteam) {
init_larry_harvey_goa_point_guard(pteam->players+0);
init_larry_harvey_goa_shooting_guard(pteam->players+1);
init_larry_harvey_goa_center(pteam->players+2);
init_larry_harvey_goa_small_forward(pteam->players+3);
init_larry_harvey_goa_power_forward(pteam->players+4);
}
  

  











	    
/* ALIENS  ALIENS  ALIENS  ALIENS  ALIENS  ALIENS  ALIENS  team eclectic ALIENS  ALIENS  ALIENS  ALIENS  ALIENS  ALIENS  ALIENS  */
void init_larry_harvey_aliens_standard(larry_harvey_robot *probot) {
probot->usual_first_color  = CLITERAL{200,80,210,255};   	// pinkish
probot->usual_second_color = CLITERAL{128,128,128,0};         	// Grey
probot->usual_third_color  = CLITERAL{201,135,113,255};  	// pants/hat
probot->number_second_color_sections = 0;			// No second color for hope
probot->second_sections = NULL;
probot->number_third_color_sections = 0;			// Only 2 teams has 3 colors.
probot->third_sections = NULL;
}
	      

		  
/*
	      XXXXXXXXXXXX
            XXXXXXXXXXXXXXXX           				  		  
        XXXXXXXXXXXXXXXXXXXXXXXX	   
    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX	   
        XXXXXXXXXXXXXXXXXXXXXXXX	   
            XXXXXXXXXXXXXXXX           				  		  
	      XXXXXXXXXXXX
*/    
	   

void init_larry_harvey_aliens_saucer(larry_harvey_robot *probot) {
init_larry_harvey_aliens_standard(probot);
probot->usual_first_color  = CLITERAL{200,200,150,255}; 

probot->number_first_color_sections = 7;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);

probot->first_sections[0].the_section = (Rectangle) {.x=-21,.y=-15,.width=42,.height=6};// top
probot->first_sections[1].the_section = (Rectangle) {.x=-27,.y=-9,.width=54,.height=6};
probot->first_sections[2].the_section = (Rectangle) {.x=-33,.y=-3,.width=66,.height=6};
probot->first_sections[3].the_section = (Rectangle) {.x=-51,.y=3,.width=100,.height=6}; 
probot->first_sections[4].the_section = (Rectangle) {.x=-33,.y=9,.width=66,.height=6}; 
probot->first_sections[5].the_section = (Rectangle) {.x=-27,.y=15,.width=54,.height=6};
probot->first_sections[6].the_section = (Rectangle) {.x=-21,.y=21,.width=42,.height=6}; 
    
}


/*
CThulu
      XXXXXX
    XXXXXXXXXX
    XXXXXXXXXX
    XX  XX  XX
  XXXXXXXXXXXXXX            
      XX  XX
      XX  XX
*/
void init_larry_harvey_aliens_cthulu(larry_harvey_robot *probot) {
init_larry_harvey_aliens_standard(probot);
probot->number_first_color_sections = 5;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-9,.y=-21,.width=18,.height=6};  // top head
probot->first_sections[1].the_section = (Rectangle) {.x=-15,.y=-15,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[2].the_section = (Rectangle) {.x=-21,.y=3,.width=42,.height=6};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[3].the_section = (Rectangle) {.x=-9,.y=6,.width=6,.height=12};   // left foot
probot->first_sections[4].the_section = (Rectangle) {.x=3,.y=6,.width=6,.height=12};   	// right foot

probot->number_second_color_sections = 0;
probot->second_sections = NULL;

probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}



/*
larry
      XXXXXX
  XXXXXXXXXXXXXX            
    XXXXXXXXXX
    XX  XX  XX
    XXXXXXXXXX
      XX  XX
      XX  XX
*/
void init_larry_harvey_aliens_larry(larry_harvey_robot *probot) {
init_larry_harvey_aliens_standard(probot);
probot->usual_first_color  = CLITERAL{201,135,113,255};  	// Mr. Harvey
probot->number_first_color_sections = 3;

probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-15,.y=-9,.width=30,.height=18};// each block is a 54x54 square with 6 pixels not used.
probot->first_sections[1].the_section = (Rectangle) {.x=-9,.y=6,.width=6,.height=12};   // left foot
probot->first_sections[2].the_section = (Rectangle) {.x=3,.y=6,.width=6,.height=12};   	// right foot


probot->usual_second_color  = CLITERAL{130,70,64,255};  
probot->number_second_color_sections = 2;
probot->second_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_second_color_sections);
probot->second_sections[0].the_section = (Rectangle) {.x=-9,.y=-21,.width=18,.height=6};  // top head
probot->second_sections[1].the_section = (Rectangle) {.x=-21,.y=-15,.width=42,.height=6};// each block is a 54x54 square with 6 pixels not used.


probot->number_third_color_sections = 0;
probot->third_sections = NULL;
}



/*
paper

    XXXXXX
    XXXXXX
    XXXXXX
    XXXXXX

*/
void init_larry_harvey_aliens_paper(larry_harvey_robot *probot) {
init_larry_harvey_aliens_standard(probot);
probot->usual_first_color  = CLITERAL{215,215,200,255};  
probot->number_first_color_sections = 1;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-9,.y=-9,.width=18,.height=18};// each block is a 54x54 square with 6 pixels not used.
}



/*
paper

    XX
    XX
    XX
    XX

*/
void init_larry_harvey_aliens_rolled_paper(larry_harvey_robot *probot) {
init_larry_harvey_aliens_standard(probot);
probot->usual_first_color  = CLITERAL{215,215,200,255};  
probot->number_first_color_sections = 1;
probot->first_sections = malloc(sizeof(larry_harvey_rectangle_section)*probot->number_first_color_sections);
probot->first_sections[0].the_section = (Rectangle) {.x=-3,.y=-9,.width=6,.height=18};// each block is a 54x54 square with 6 pixels not used.
}





void init_larry_harvey_aliens(larry_harvey_basketball_team *pteam) {
init_larry_harvey_aliens_saucer(pteam->players+0);
init_larry_harvey_aliens_cthulu(pteam->players+1);
init_larry_harvey_aliens_larry(pteam->players+2);
init_larry_harvey_aliens_paper(pteam->players+3);
init_larry_harvey_aliens_rolled_paper(pteam->players+4);
}
  
  
    
      /* note - there are a couple more part of the "pink"group - a ninth group

      XX
      XXXXXX
    XXXXXXXXXX
  XXXX  XX  XXXX
    XXXXXXXXXX
    XXXXXXXXXX    
      XX  XX
      


*/
      
























/* THE ENTIRE LEAGUE -------------------------------------------- THE ENTIRE LEAGUE */

larry_harvey_robot_league * new_larry_harvey_robot_league() {
larry_harvey_robot_league *league=malloc(sizeof(larry_harvey_robot_league));

init_larry_harvey_evil(league->teams+0);
init_larry_harvey_love(league->teams+1);
init_larry_harvey_burner(league->teams+2);
init_larry_harvey_star(league->teams+3);

init_larry_harvey_screw(league->teams+4);
init_larry_harvey_pasta(league->teams+5);
init_larry_harvey_hope(league->teams+6);
init_larry_harvey_goa(league->teams+7);

init_larry_harvey_aliens(league->teams+8);

return league;
}


void free_larry_harvey_robot_league(larry_harvey_robot_league *league) {
// nah
}











































/* DRAWING DRAWING DRAWING */
#ifdef L8TR
void draw_larry_harvey_robot_2d(larry_harvey_robot *probot,Vector2 translate,Vector2 scale) {
const float factor= 1.f/56.f;

int i;
for (i=0;i<probot->number_first_color_sections;i++) {
  Rectangle r= probot->first_sections[i].the_section;
  Vector2 positioned={.x = ((float)r.x)*scale.x*factor + translate.x,
                      .y = ((float)r.y)*scale.y*factor + translate.y};
  Vector2 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor};
  DrawRectangleV(positioned,geometry,probot->usual_first_color);
  }

for (i=0;i<probot->number_second_color_sections;i++) {
  Rectangle r= probot->second_sections[i].the_section;
  Vector2 positioned={.x = ((float)r.x)*scale.x*factor + translate.x,
                      .y = ((float)r.y)*scale.y*factor + translate.y};
  Vector2 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor};
  DrawRectangleV(positioned,geometry,probot->usual_second_color);
  }


for (i=0;i<probot->number_third_color_sections;i++) {
  Rectangle r= probot->third_sections[i].the_section;
  Vector2 positioned={.x = ((float)r.x)*scale.x*factor + translate.x,
                      .y = ((float)r.y)*scale.y*factor + translate.y};
  Vector2 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor};
  DrawRectangleV(positioned,geometry,probot->usual_third_color);
  }

{
  DrawCircleV((Vector2){.x=7.f * scale.x * factor+translate.x,  .y= 0.f * scale.y* factor+translate.y}, 5.5f * scale.x*factor,BLACK);
  DrawCircleV((Vector2){.x=-7.f* scale.x * factor+translate.x,  .y= 0.f * scale.y* factor+translate.y}, 5.5f * scale.x*factor,BLACK);
  DrawCircleV((Vector2){.x=5.f * scale.x * factor+ translate.x, .y= -2.f * scale.y * factor+translate.y},2.5f *scale.x * factor,WHITE);
  DrawCircleV((Vector2){.x=-5.f * scale.x *factor+translate.x,  .y= -2.f * scale.y * factor+translate.y},2.5f *scale.x * factor,WHITE);
  DrawCircleV((Vector2){.x=9.f  * scale.x * factor+translate.x, .y= 2.f * scale.y * factor+translate.y},1.333f *scale.x * factor,WHITE);
  DrawCircleV((Vector2){.x=-9.f * scale.x * factor+translate.x, .y= 2.f * scale.y * factor+translate.y},1.333f *scale.x * factor,WHITE);
  }

}

#endif


#define RST_LARRY(aa,factor,translate,scale,sinangle,cosangle) (Vector3){ \
   .x=(aa.x*cosangle+aa.y*sinangle)*scale.x*factor + translate.x, \
   .y=(aa.y*cosangle-aa.x*sinangle)*scale.y*factor + translate.y, \
   .z=(aa.z*scale.z*factor + translate.z)}

#define SRT_LARRY(aa,factor,translate,scale,sinangle,cosangle) (Vector3){ \
   .x=(aa.x*cosangle*scale.x+aa.y*sinangle*scale.y)*factor + translate.x, \
   .y=(aa.y*cosangle*scale.y-aa.x*sinangle*scale.x)*factor + translate.y, \
   .z=(aa.z*scale.z*factor + translate.z)}

#define RT_LARRY(aa,translate,sinangle,cosangle) (Vector3){ \
   .x=(aa.x*cosangle+aa.y*sinangle) + translate.x, \
   .y=(aa.y*cosangle-aa.x*sinangle) + translate.y, \
   .z=(aa.z + translate.z)}

#define TR_LARRY(aa,translate,sinangle,cosangle) (Vector3){ \
   .x=((aa.x+translate.x)*cosangle+(aa.y+translate.y)*sinangle), \
   .y=((aa.y+translate.y)*cosangle-(aa.x+translate.x)*sinangle), \
   .z=(aa.z + translate.z)}



unsigned int letters_opengles_color_handle(void);





// Draw a circle in 3D world space
void DrawFilledCircle3D(Vector3 center, float radius, Vector3 rotationAxis, float rotationAngle, Color color)
{
#ifdef LARRY_GLES_2_DIRECT

//lg_set_scale_translate(1.f,0.f,0.f,0.f);
/* we ignore the rotation*/
glUniform4f(letters_opengles_color_handle(), ((float)color.r)*0.00390625f,((float)color.g)*0.00390625f,((float)color.b)*0.00390625f,1.0f);
     checkGlError("glUniform4ia");
void lg_circle_notransform(int segments,fl circleSize,lp center);
lg_set_scale_translate(1.f,0.f,0.f,center.z);
lg_circle_notransform(20,radius,(lp) {center.x,center.y});
     checkGlError("glcnt2");


	    
#else
  /* raylib */
   rlPushMatrix();
        rlTranslatef(center.x, center.y, center.z);
        rlRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
 
        rlBegin(RL_TRIANGLES);
            for (int i = 0; i < 360; i += 20)
            {

                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(sinf(DEG2RAD*i)*radius, cosf(DEG2RAD*i)*radius, 0.0f);
                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(0.f,0.f,0.f);
                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(sinf(DEG2RAD*(i + 20))*radius, cosf(DEG2RAD*(i + 20))*radius, 0.0f);

            }
        rlEnd();
    rlPopMatrix();
#endif    
}











// Draw a quad in 3d
void DrawRotatedRectangleV(Vector3 position, Vector3 size,Vector3 translate, float sinangle,float cosangle, Color color)
{
#ifdef LARRY_GLES_2_DIRECT
glUniform4f(letters_opengles_color_handle(), ((float)color.r)*0.00390625f,((float)color.g)*0.00390625f,((float)color.b)*0.00390625f,1.0f);
     checkGlError("glUniform4issds");
    size.x = -0.5*size.x;    size.y = -0.5*size.y; size.z = -0.5*size.z;    


		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
		Vector3 p3;
		//Vector3 t3 = {0.f,0.f,0.f};
		p0 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p0 = RT_LARRY(p0,translate,sinangle,cosangle);
		size.x = -size.x;
		p1 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p1 = RT_LARRY(p1,translate,sinangle,cosangle);
                size.y = -size.y;
		p3 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p3 = RT_LARRY(p3,translate,sinangle,cosangle);
                size.x = -size.x;
		p2 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p2 = RT_LARRY(p2,translate,sinangle,cosangle);

  GLfloat vertices[] = { p0.x,p0.y,p0.z,p3.x,p3.y,p3.z,p1.x,p1.y,p1.z,
                         p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,p0.x,p0.y,p0.z};
  GLfloat vertices2[] = { p3.x,p3.y,p3.z,p0.x,p0.y,p0.z,p1.x,p1.y,p1.z,
                         p3.x,p3.y,p3.z,p2.x,p2.y,p2.z,p0.x,p0.y,p0.z};

  unsigned int mPositionHandle=letters_opengles_position_handle();
  glEnableVertexAttribArray(mPositionHandle);
checkGlError("glea1");
  glVertexAttribPointer(mPositionHandle, 3,
                                 GL_FLOAT, GL_FALSE,12
                                 ,vertices);
  // Draw the triangle
checkGlError("glvap1");
  glDrawArrays(GL_TRIANGLES, 0, 6);
checkGlError("dgat1");


  glVertexAttribPointer(mPositionHandle, 3,
                                 GL_FLOAT, GL_FALSE,12
                                 ,vertices2);
  // Draw the triangle
checkGlError("glvap1");
  glDrawArrays(GL_TRIANGLES, 0, 6);
checkGlError("dgat1");


  // Disable vertex array
  glDisableVertexAttribArray(mPositionHandle);
checkGlError("ddaa1");
  
#else	
    rlPushMatrix();
//        rlTranslatef(translate.x, translate.y, translate.z);

    size.x = -0.5*size.x;    size.y = -0.5*size.y; size.z = 0.5*size.z;    
 

        rlBegin(RL_TRIANGLES);
		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
		Vector3 p3;
		Vector3 t3 = {0.f,0.f,0.f};
		p0 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p0 = RT_LARRY(p0,translate,sinangle,cosangle);
		size.x = -size.x;
		p1 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p1 = RT_LARRY(p1,translate,sinangle,cosangle);
                size.y = -size.y;
		p3 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p3 = RT_LARRY(p3,translate,sinangle,cosangle);
                size.x = -size.x;
		p2 = (Vector3){position.x+size.x,position.y+size.y,position.z+size.z};
		p2 = RT_LARRY(p2,translate,sinangle,cosangle);

 
                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(p0.x,p0.y,p0.z);
                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(p1.x,p1.y,p1.z);
                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(p3.x,p3.y,p3.z);


                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(p3.x,p3.y,p3.z);
                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(p2.x,p2.y,p2.z);
                rlColor4ub(color.r, color.g, color.b, color.a);
                rlVertex3f(p0.x,p0.y,p0.z);

        rlEnd();
    rlPopMatrix();
#endif    
}












void draw_larry_harvey_robot_container_3d_rotate(larry_harvey_robot *probot,Vector3 translate,Vector3 scale,float sinangle,float cosangle) {
const float factor= 1.f/56.f;

int i;
i=0;
{

  Rectangle r= probot->first_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,probot->usual_first_color);
  }

}


static Color sparkle_color(Color in) {
Color out;
int cmax=0;
if (in.r>cmax) cmax= in.r;
if (in.g>cmax) cmax= in.g;
if (in.b>cmax) cmax= in.b;
if (cmax ==0) cmax=200;
float f1;
f1 = 255.f / ((float)cmax) ;
int o;
o = ((float)in.r) * f1 + 32.f;
if (o>255) o=255;
out.r = o;

o = ((float)in.g) * f1 + 32.f;
if (o>255) o=255;
out.g = o;

o = ((float)in.b) * f1 + 32.f;
if (o>255) o=255;
out.b = o;
return out;
}




void draw_larry_harvey_robot_container_3d_sparkle(larry_harvey_robot *probot,Vector3 translate,Vector3 scale) {
const float factor= 1.f/56.f;
const float sinangle=0.f;
const float cosangle=0.f;
int i;
i=0;
{

  Rectangle r= probot->first_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,sparkle_color(probot->usual_first_color));
  }
}




void draw_larry_harvey_robot_container_3d(larry_harvey_robot *probot,Vector3 translate,Vector3 scale) {
draw_larry_harvey_robot_container_3d_rotate(probot,translate,scale,0.f,1.f);
}

   


void draw_larry_harvey_robot_3d_rotate(larry_harvey_robot *probot,Vector3 translate,Vector3 scale,float sinangle,float cosangle) {
const float factor= 1.f/56.f;

  

{
  const Vector3 c1 = (Vector3){.x=7.f,.y=0.f,.z=0.2};
  DrawFilledCircle3D(SRT_LARRY(c1,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,BLACK);
  const Vector3 c2 = (Vector3){.x=-7.f,.y=0.f,.z=0.2};
  DrawFilledCircle3D(SRT_LARRY(c2,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,BLACK);
  const Vector3 c3 = (Vector3){.x=5.f,.y=2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c3,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  const Vector3 c4 = (Vector3){.x=-5.f,.y=2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c4,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  const Vector3 c5 = (Vector3){.x=9.f,.y=-2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c5,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  const Vector3 c6 = (Vector3){.x=-9.f,.y=-2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c6,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  }

int i;
for (i=0;i<probot->number_first_color_sections;i++) {

  Rectangle r= probot->first_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,probot->usual_first_color);
  }

for (i=0;i<probot->number_second_color_sections;i++) {
  Rectangle r= probot->second_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,probot->usual_second_color);
  }


for (i=0;i<probot->number_third_color_sections;i++) {
  Rectangle r= probot->third_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,probot->usual_third_color);
  }


{
  const Vector3 c1 = (Vector3){.x=7.f,.y=0.f,.z=-0.2};
  DrawFilledCircle3D(SRT_LARRY(c1,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,BLACK);
  const Vector3 c2 = (Vector3){.x=-7.f,.y=0.f,.z=-0.2};
  DrawFilledCircle3D(SRT_LARRY(c2,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,BLACK);
  const Vector3 c3 = (Vector3){.x=5.f,.y=2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c3,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  const Vector3 c4 = (Vector3){.x=-5.f,.y=2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c4,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  const Vector3 c5 = (Vector3){.x=9.f,.y=-2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c5,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  const Vector3 c6 = (Vector3){.x=-9.f,.y=-2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c6,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,WHITE);
  }

  

}






void draw_larry_harvey_robot_3d(larry_harvey_robot *probot,Vector3 translate,Vector3 scale) {
draw_larry_harvey_robot_3d_rotate(probot,translate,scale,0.f,1.f);
}




void draw_larry_harvey_robot_3d_no_eyes_rotate_colors(larry_harvey_robot *probot,
Vector3 translate,
Vector3 scale,
float sinangle,float cosangle,
Color color1,
Color color2,
Color color3) {
const float factor= 1.f/56.f;

int i;
for (i=0;i<probot->number_first_color_sections;i++) {

  Rectangle r= probot->first_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,
    color1);
  }

for (i=0;i<probot->number_second_color_sections;i++) {
  Rectangle r= probot->second_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,
    color2);
  }


for (i=0;i<probot->number_third_color_sections;i++) {
  Rectangle r= probot->third_sections[i].the_section;
  Vector3 positioned={.x = ( ((float)r.x) + ((float)(r.width))*0.5)*scale.x*factor,
                      .y = ( ((float)-r.y) -((float)r.height)*0.5)*scale.y*factor,
		      .z = ((float)0.f)*scale.z*factor};
  Vector3 geometry={.x=((float)r.width)*scale.x*factor,
                .y=((float)r.height)*scale.y*factor,
		.z=((float)0.0)*scale.z*factor};  
  DrawRotatedRectangleV(positioned,geometry,translate,sinangle,cosangle,
  color3);
  }
}








void draw_larry_harvey_robot_eyes_only_3d_rotate(
  larry_harvey_robot *probot,Vector3 translate,Vector3 scale,
  float sinangle,float cosangle,Color eye_white, Color eye_yolk) {
const float factor= 1.f/56.f;

  

{
  const Vector3 c1 = (Vector3){.x=7.f,.y=0.f,.z=0.2};
  DrawFilledCircle3D(SRT_LARRY(c1,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_white);
  const Vector3 c2 = (Vector3){.x=-7.f,.y=0.f,.z=0.2};
  DrawFilledCircle3D(SRT_LARRY(c2,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_white);
  const Vector3 c3 = (Vector3){.x=5.f,.y=2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c3,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  const Vector3 c4 = (Vector3){.x=-5.f,.y=2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c4,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  const Vector3 c5 = (Vector3){.x=9.f,.y=-2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c5,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  const Vector3 c6 = (Vector3){.x=-9.f,.y=-2.f,.z=0.4f};
  DrawFilledCircle3D(SRT_LARRY(c6,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  }


{
  const Vector3 c1 = (Vector3){.x=7.f,.y=0.f,.z=-0.2};
  DrawFilledCircle3D(SRT_LARRY(c1,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_white);
  const Vector3 c2 = (Vector3){.x=-7.f,.y=0.f,.z=-0.2};
  DrawFilledCircle3D(SRT_LARRY(c2,factor,translate,scale,sinangle,cosangle),
    5.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_white);
  const Vector3 c3 = (Vector3){.x=5.f,.y=2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c3,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  const Vector3 c4 = (Vector3){.x=-5.f,.y=2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c4,factor,translate,scale,sinangle,cosangle),
    2.5f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  const Vector3 c5 = (Vector3){.x=9.f,.y=-2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c5,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  const Vector3 c6 = (Vector3){.x=-9.f,.y=-2.f,.z=-0.4f};
  DrawFilledCircle3D(SRT_LARRY(c6,factor,translate,scale,sinangle,cosangle),
    1.333f * scale.x*factor,
    (Vector3){.x=0.f,.y=0.f,.z=1.f},0.f,eye_yolk);
  }

  

}



/* End of file larry_harvey_has_a_possee */

  

