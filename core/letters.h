/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2018 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Library Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Library Public License for more details.

You should have received a copy of the GNU Library Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef LETTERS_MAIN_H
#define LETTERS_MAIN_H


typedef float GLfloat;
typedef GLfloat fl;

typedef struct lp {
  fl x;
  fl y;
  } lp;

typedef struct lr {
  lp lower_left;
  lp upper_right;
  } lr;

typedef struct li {
  lr boundary;
  } li;
  

struct lcontext;

typedef struct lneeded_points { 
  fl standard_kern;
  lp topmid;
  lp topright;
  lp bottommid;
  lp bottomleft;
  lp bottomright;
  lp mid_A_left;
  lp mid_A_right;
  fl min_glyph_width; // used by !
  fl line_width;
  lp dot;
  fl dot_width;
  lp bottom_exclamation;
  lp top_exclamation;
  lp midsize_top;
  lp midsize_bottom;
  lp topleft;
  
  lp double_quote_l1_p1;
  lp double_quote_l1_p2;
  lp double_quote_l2_p1;
  lp double_quote_l2_p2;
  
  
  lp hash_l1_p1;
  lp hash_l1_p2;
  lp hash_l2_p1;
  lp hash_l2_p2;
  lp hash_l3_p1;
  lp hash_l3_p2;
  lp hash_l4_p1;
  lp hash_l4_p2;

  lp dollar_circle1;
  lp dollar_circle2;
  fl dollar_circle_radius;
  lp dollar_top;
  lp dollar_bottom;
  
  lp percent_circle1;
  lp percent_circle2;
  fl percent_circle_radius;
  
  lp single_quote_l1_p1;
  lp single_quote_l1_p2;
  
  lp openparen_circle1;
  fl openparen_circle_radius;
  lp openparen_upper_right;
  
  lp closeparen_circle1;
  fl closeparen_circle_radius;
  
  lp star_l1_p1;
  lp star_l1_p2;
  lp star_l2_p1;
  lp star_l2_p2;
  lp star_l3_p1;
  lp star_l3_p2;
  
  lp plus_l1_p1;
  lp plus_l1_p2;
  lp plus_l2_p1;
  lp plus_l2_p2;
  
  lp comma_l1_p1;
  lp comma_l1_p2;
  
  lp minus_l1_p1;
  lp minus_l1_p2;
  
  lp one_p1;
  lp one_p2;
  
  lp two_circle;
  fl two_circle_radius;
  lp two_circle_l1_p1;
  lp two_circle_l1_p2;
  
  lp four_l1_p1;
  lp four_l1_p2;
  lp four_l1_p3;
  lp four_l1_p4;
      
  lp five_l1_p1;
  lp five_l1_p2;
  lp five_l1_p3;
  lp five_l1_p4;
  lp five_circle;
  fl five_circle_radius;
  
  lp six_circle2;
  fl six_circle2_radius;
  lp six_p1;
  lp six_p2;
  
  lp seven_p1;
  lp seven_p2;
  lp seven_p3;
  
  lp eight_circle2;
  fl eight_circle_radius;
  
  lp nine_p1;
  lp nine_p2;
  lp nine_circle2;
  fl nine_circle2_radius;
  
  lp colon_p1;
  lp colon_p2;
  
  lp semicolon_p1;
  lp semicolon_p2;
  lp semicolon_p3;
  
  lp lessthan_p1;
  lp lessthan_p2;
  lp lessthan_p3;

  lp equals_l1_p1;
  lp equals_l1_p2;
  lp equals_l2_p1;
  lp equals_l2_p2;
  
  lp greaterthan_p1;
  lp greaterthan_p2;
  lp greaterthan_p3;
  
  lp question_mark_dot;
  lp question_mark_p1;
  lp question_mark_p2;

    
  lp at_circle1;
  fl at_circle1_radius;
  lp at_l1_p1;
  lp at_l1_p2;
  lp at_circle2;
  fl at_circle2_radius;
  fl at_circle3_radius;
  
  lp circle_B_top;
  fl circle_BPR_radius;
  lp circle_B_bottom;
  lp topright_B;
  lp circle_C;  // used by many
  fl circle_C_radius; 
  lp halfsize_top;
  lp halfsize_bottom;
  lp circle_D;
  lp EF_topright;
  lp EFH_midleft;
  lp EF_midright;
  lp E_bottomright;
  lp circle_G;
  fl circle_G_radius;
  lp G_l3bot;
  lp G_l3top;
  lp G_l4left;
  lp G_l4right;
  lp H_rightmid;
  lp I_topright;
  lp I_bottomright;
  lp I_topbar;
  lp I_bottombar;
  lp J_topright;
  lp J_bottombar;
  lp J_circle;
  fl J_circle_radius;
  lp Q1;
  lp S_circle1;
  lp S_circle2;
  fl S_circle_radius;
  lp S_l1right;
  lp S_l2right;
  lp T_topbar;
  lp TVY_bottombar;
  lp U_l1bot;
  lp U_l2bot;
  lp U_circle1;
  fl U_circle_radius;
  lp W_p1;
  lp W_p2;
  lp Y_center;
  lp carat_l1;
  lp carat_l2;
  lp carat_l3;
  lp backquote_l1;
  lp lower_circle;
  fl lower_circle_radius;
  lp a_l1;
  lp c_l1;
  lp c_l2;
  lp e_l1;
  lp e_l2;
  lp e_l3;
  lp e_l4;
  lp ft_bartop;
  lp ft_barbottom;
  lp f_l1;
  lp f_l2;
  lp f_circle;
  fl f_circle_radius;
  lp f_l3;
  lp g_l1;
  lp g_l2;
  lp g_circle;
  fl g_circle_radius;
  lp h_l1;
  lp i_bottom;
  lp i_top;
  lp i_dot;
  lp j_l1;
  lp j_l2;
  lp j_circle;
  fl j_circle_radius;
  lp j_dot;
  lp k_l1;
  lp k_l2;
  lp k_l3;
  lp mnpr_topleft;
  lp m_l1;
  lp m_l2;
  lp m_l3;
  lp m_l4;
  lp m_circle1;
  fl m_circle_radius;
  lp m_circle2;
  lp nr_circle1;
  fl nr_circle_radius;
  lp nr_l1;
  lp n_l2;
  lp p_bottombar;  
  lp q_circle;
  fl q_circle_radius;
  lp q_l3;
  lp s_circle1;
  lp s_circle2;
  fl s_circle_radius;
  lp s_l1left;
  lp s_l1right;
  lp s_l2right;
  lp s_extra_spot;
  lp t_l1;
  lp t_l2;
  lp uvwxyz_topleft;
  lp u_p1;
  lp v_p1;
  lp w_p1;
  lp w_p2;
  lp w_p3;
  lp w_p4;
  lp y_p1;
  lp close_brace_circle_1;
  lp close_brace_circle_2;
  lp close_brace_circle_3;
  lp close_brace_circle_4;
  fl close_brace_inside_radius;
  fl close_brace_outside_radius;
  lp close_brace_midpoint;  
  lp braces_l1_p1;
  lp braces_l1_p2;
  lp braces_l2_p1;
  lp braces_l2_p2;
  lp open_brace_circle_1;
  lp open_brace_circle_2;
  lp open_brace_circle_3;
  lp open_brace_circle_4;
  fl open_brace_inside_radius;
  fl open_brace_outside_radius;
  lp open_brace_toppoint;  
  lp open_brace_midpoint;  
  lp open_brace_bottompoint;  
  
  lp vertical_bar_p1;
  lp vertical_bar_p2;
  lp vertical_bar_p3;
  lp vertical_bar_p4;
  lp tilde_circle1;
  lp tilde_circle2;
  fl tilde_circle_radius;
  
  lp del_p1;
  lp del_p2;
  lp del_p3;
  lp del_p4;
  lp del_p5;
  lp del_p6;
  
  } lneeded_points;

  
  
typedef struct linterface {
int (*lsize[256])(struct lcontext *lc);
int (*ldraw[256])(lneeded_points *lnp);
} linterface;
// draw interface is hard coded


// helpers for non-op chaaracters to be implemeted in the interface
#define ls_NONOP(lc)  {lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = {0.f,0.f}}; return 1;}
#define ld_NONOP(lc)  {return 1;}
// whhy is this different two nonops?
	  
	  
#define LSTYLE_NORMAL 1 
// for needed points	  
	  
	  
typedef struct lcontext {
  lneeded_points *needed_points;
  linterface *interface;
  lr next_character_boundary;
  char next_character;
  lr previous_character_boundary;
  char previous_character;
  lr horizontal_string_boundary;
  lr vertical_string_boundary;
  lr max_boundary;
  int cursor;
  lp point_cursor;
  fl scale; /* minus the below stuff */
  lp start_point;
  fl start_point_z;
  int word_wrap_flag; /* 1 if we can wrap below */
} lcontext;


            

extern int lcan_add_boundary(lcontext *lc);
/* returns true if we can add a boundary section for a character without going over a range
 also sets the cursor position to lower left corner */

		  
extern int ladd_boundary(lcontext *lc);
/* returns true if we can add a boundary section for a character without going over a range
but more importantly, this sets the cursor_point to after the drawn character
*/


extern void letters_out(lcontext *lc,fl scale,fl x,fl y,fl z,char *string);
extern lcontext * linit_context(void);






// tie ins to opengles and ascii
extern void lg_set_scale_translate(fl scale,fl x,fl y, fl z);

extern void lg_start_drawing(lcontext *lc);
extern void lg_wide_line(lp p1,lp p2,fl line_width);
extern void lg_circle(int segments,fl circleSize,lp center);
extern void lg_wide_arc(int segments,fl circleSize,lp center,fl startDegrees,fl lengthDegrees,fl lineWidth,int drawStartCap, 
    int drawEndCap);
    







extern  linterface letters_ascii_interface;
extern  lneeded_points *linit_ascii_normal_points(lneeded_points *lnp);
extern float lkern_ascii(lcontext *lc);												  



extern void letters_size(lcontext *lc,fl scale,fl x,fl y,fl z,char *string);

#endif

