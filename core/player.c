





static int game_init(hate_game *game,int my_player_id) {
game->dist = 0.6666666666666666666;
game->my_player_id = my_player_id;
hate_screens *screens = &(game->screens);
int *ms;
ms = superpos_map_to(0);


/* init screens */
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  glMatrix range;
  glMatrix ex;
  screens->enabled[i]=0;
  hate_screen *screen = &(screens->screen[i]);
  screen->player_id=1;
  if (i==0) {
    loadIdentity(&range);
    scaleMatrix(&range,dist/4.0;dist/4.0,dist/4.0);    
    screen->screenViewMatrix = range;
    }
  else if (i==1) {
    loadIdentity(&range);
    translateMatrix(&range,1.f,1.f);
    scaleMatrix(&range,dist/4.0;dist/4.0,dist/4.0); 
    screen->screenViewMatrix = range;
    }
  else if (i==2) {
    loadIdentity(&range);
    translateMatrix(&range,1.f,-1.f);
    scaleMatrix(&range,dist/4.0;dist/4.0,dist/4.0); 
    screen->screenViewMatrix = range;
    }
  else if (i==3) {
    loadIdentity(&range);
    translateMatrix(&range,-1.f,-1.f);
    scaleMatrix(&range,dist/4.0;dist/4.0,dist/4.0); 
    screen->screenViewMatrix = range;
    }
  else if (i==4) {
    loadIdentity(&range);
    translateMatrix(&range,-1.f,+1.f);
    scaleMatrix(&range,dist/4.0;dist/4.0,dist/4.0); 
    screen->screenViewMatrix = range;
    }
  }
/* init players */
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  hate_player player=game->players[i];
  player->player_id=i;
  for (int j=0;j<HATE_NUMBER_ROBOTS;j++) {
    hate_robot *robot = &(player->robots[j]);
    robot->current_point=(superpoint){{xyz:0.f,0.f,0.f}};
    robot->last_point=(superpoint){{xyz:0.f,0.f,0.f}};
    robot->size=0.07;
    robot->sizetimessize=robot->size*robot->size;
    robot->need_to_show_ghost_last_point=0;
    robot->need_to_show_ghost_first_point=0;
    robot->current_ghost_point=(superpoint){{xyz:0.f,0.f,0.f}};
    robot->last_ghost_point=(superpoint){{xyz:0.f,0.f,0.f}};
    }
  }
}




static int player_step(hate_game *game) {
for (int i=0;i<HATE_NUMBER_PLAYERS;i++) {
  hate_player player=game->players[i];   
  for (int j=0;j<HATE_NUMBER_ROBOTS;j++) {
    hate_robot *robot = &(player->robots[j]);
    }  
  }
}



static int player_draw(hate_game *game) {
for (int s=0;s<HATE_NUMBER_PLAYERS;s++) {
  if (game->screens->enabled[s]) {
    hate_screen screen = &(game->screens.screens[s]);
    
    for (int i=0;i<1;i++) {
    
      hate_player player=game->players[i];   
      for (int j=0;j<1;j++) {
        hate_robot *robot = &(player->robots[j]);
        player_draw_robot(game,player,robot,
        }
      }  
    }
}
