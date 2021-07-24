#include <stdio.h>

#include "letters_ascii.h"



lneeded_points *linit_ascii_normal_points(lneeded_points *lnp) {
*lnp = (lneeded_points) {.standard_kern=0.15f,
                         .topmid = (lp) {0.5f,1.0f},
			 .topright = (lp) {1.f,1.f},
			 .bottommid = (lp) {0.5f,0.f},
                         .bottomleft = (lp) {0.0f,0.0f},
                         .bottomright = (lp) {1.0f,0.0f},
                         .mid_A_left = (lp) {0.25f,0.5f},
                         .mid_A_right = (lp) {0.75f,0.5f},
                         .min_glyph_width=0.08f,
                         .line_width=0.04f,
                         .dot = (lp) { 0.00f,0.00f},
                         .dot_width = 0.04f,
                         .bottom_exclamation = (lp) {0.0f,0.2f},
                         .top_exclamation = (lp) {0.0f,0.9f},
			 .midsize_top = (lp) {0.6f,1.f},
			 .midsize_bottom = (lp) {0.6f,0.f},			 
			 .topleft = (lp) {0.0f,1.0f},
			 
			 
			 .double_quote_l1_p1 = (lp) {0.f,1.f},
			 .double_quote_l1_p2 = (lp) {0.f,0.8f},
			 .double_quote_l2_p1 = (lp) {0.2f,1.f},
			 .double_quote_l2_p2 = (lp) {0.2f,0.8f},
			 
			 
			 .hash_l1_p1 = (lp) {0.f,0.60f},
			 .hash_l1_p2 = (lp) {0.6f,0.60f},
			 .hash_l2_p1 = (lp) {0.f,0.40f},
			 .hash_l2_p2 = (lp) {0.6f,0.40f},
			 .hash_l3_p1 = (lp) {0.2f,0.2f},
			 .hash_l3_p2 = (lp) {0.2f,0.8f},
			 .hash_l4_p1 = (lp) {0.4f,0.2f},
			 .hash_l4_p2 = (lp) {0.4f,0.8f},
			 
			 .dollar_circle1 = (lp) {0.20f,0.70f},
			 .dollar_circle2 = (lp) {0.20f,0.30f},
			 .dollar_circle_radius = 0.20f,
			 .dollar_top = (lp) {0.20f,1.f},
			 .dollar_bottom = (lp) {0.20f,0.f},
			 
			 .percent_circle1 = (lp) {0.8f,0.2f},
			 .percent_circle2 = (lp) {0.2f,0.8f},
			 .percent_circle_radius =  0.2f,
			 
			 // ampersand is basically dollar 
			 
			 .single_quote_l1_p1 = (lp) {0.f,1.f},
			 .single_quote_l1_p2 = (lp) {0.f,0.8f},
			 
			 .openparen_circle1 = (lp) {1.f,0.5f},
			 .openparen_circle_radius = 1.f,
			 .openparen_upper_right = (lp) {0.133397459,1.}, // 1-sqrt(3)/2 ,1 
			 
			 .closeparen_circle1 = (lp) {-0.8660254037f,0.5f},   // -sqrt(3)/2, 0.5
			 .closeparen_circle_radius = 1.f,
			 
			 .star_l1_p1 = (lp) {0.25f,0.75f},
			 .star_l1_p2 = (lp) {0.25f,0.25f},
			 .star_l2_p1 = (lp) {0.0334936490550f,0.625f},  // .25 - (cos(30)*0.25) for x,   sin(30) is 0.5 so easy to figure out
			 .star_l2_p2 = (lp) {0.4665063509450,0.375f},   // .25 + (cos(30)*0.25)
			 .star_l3_p1 = (lp) {0.0334936490550f,0.375f},  // flip em
			 .star_l3_p2 = (lp) {0.4665063509450,0.625f}, 
			 
			 .plus_l1_p1 = (lp) {0.30f,0.80f},
			 .plus_l1_p2 = (lp) {0.30f,0.20f},
			 .plus_l2_p1 = (lp) {0.60f,0.50f},
			 .plus_l2_p2 = (lp) {0.0f,0.50f},
			 
			 .comma_l1_p1 = (lp) {0.f,-0.1f},
			 .comma_l1_p2 = (lp) {0.2f,0.1f},
			 
			 
			 .minus_l1_p1 = (lp) {0.60f,0.50f},
			 .minus_l1_p2 = (lp) {0.0f,0.50f},
			 
			 .one_p1 = (lp) {0.15f,1.f},
			 .one_p2 = (lp) {0.f,0.85f},
			 
			 .two_circle = (lp) {0.3,0.7},
			 .two_circle_radius = 0.3,
			 .two_circle_l1_p1 = (lp) {0.5038f,0.48135f}, // guestimated
			 .two_circle_l1_p2 = (lp) {0.0,0.0},
			 
			 .four_l1_p1 = (lp) {0.5f,0.0},
			 .four_l1_p2 = (lp) {0.5f,1.0},
			 .four_l1_p3 = (lp) {0.f,0.35f},
			 .four_l1_p4 = (lp) {0.6f,0.35f},
			 
			 .five_l1_p1 = (lp) {0.5f,1.0f},
			 .five_l1_p2 = (lp) {0.0f,1.0f},
			 .five_l1_p3 = (lp) {0.0f,0.5f},
			 .five_l1_p4 = (lp) {0.25f,0.5f},
			 .five_circle = (lp) {0.25f,0.25f},
			 .five_circle_radius = 0.25f,
			 
			 // five circle is shared with six
			 .six_circle2 = (lp) {0.5f,0.5f},
			 .six_circle2_radius = 0.5f,
			 .six_p1 = (lp) {0.f,0.25f},
			 .six_p2 = (lp) {0.f,.5f},
			 
			 .seven_p1 = (lp) {0.f,1.f},
			 .seven_p2 = (lp) {0.5f,1.f},
			 .seven_p3 = (lp) {0.f,0.f},
			 
			 // five circle is also shared by 8
			 .eight_circle2 = (lp) {0.25f,0.75f},
			 .eight_circle_radius = 0.25f,
			 
			 .nine_p1 = (lp) {0.5f,0.75f},
			 .nine_p2 = (lp) {0.5f,0.5f},
			 .nine_circle2 = (lp) {0.f,0.5f},
			 .nine_circle2_radius = 0.5f,
			 
			 .colon_p1 = (lp) {0.0f,0.6f},
			 .colon_p2 = (lp) {0.0f,0.2f},
			 
			 .semicolon_p1 = (lp) {0.2f,0.6f},
			 .semicolon_p2 = (lp) {0.2f,0.2f},
			 .semicolon_p3 = (lp) {0.0f,0.0f},
			 
			 .lessthan_p1 = (lp) {0.6f,0.8f},
			 .lessthan_p2 = (lp) {0.f,0.5f},
			 .lessthan_p3 = (lp) {0.6f,0.2f},
			 
			 .equals_l1_p1 = (lp) {0.f,0.6f},
			 .equals_l1_p2 = (lp) {0.5f,0.6f},
			 .equals_l2_p1 = (lp) {0.f,0.4f},
			 .equals_l2_p2 = (lp) {0.5f,0.4f},
			 
			 .greaterthan_p1 = (lp) {0.0f,0.8f},
			 .greaterthan_p2 = (lp) {0.6f,0.5f},
			 .greaterthan_p3 = (lp) {0.0f,0.2f},
			 
			 
			 .question_mark_dot = (lp) {0.25f,0.0f},
			 .question_mark_p1 = (lp) {0.25f,0.2f},
			 .question_mark_p2 = (lp) {0.25f,0.5f},
			 // uses 8 circle 
			
			
			 .at_circle1 = (lp){0.5f,0.5f},
			 .at_circle1_radius = 0.25f,
			 .at_l1_p1 = (lp) {0.75f,0.7225f},
			 .at_l1_p2 = (lp) {0.75f,0.25f},
			 
			 .at_circle2 = (lp) {0.75f,0.5f},
			 .at_circle2_radius = 0.25f,
			 
			 .at_circle3_radius = 0.5f,
			 
			 .circle_B_top = (lp) {0.0f,0.75f},
			 .circle_BPR_radius = 0.25f,
			 .circle_B_bottom = (lp) {0.0f,0.25f},
			 .topright_B = (lp) {0.25f,1.},
			 .circle_C = (lp) {0.5f,0.5f},
			 .circle_C_radius = 0.5f,
			 .halfsize_top = (lp) {0.5f,1.f},
			 .halfsize_bottom = (lp) {0.5f,0.f},
			 .circle_D = (lp) {0.f,0.5f},
			 .EF_topright = (lp) {0.5f,1.f},
			 .EFH_midleft = (lp) {0.f,0.5f},
			 .EF_midright = (lp) {0.4f,0.5f},
			 .E_bottomright = (lp) {0.5f,0.f},
			 .circle_G = (lp) {0.5f,0.1f},
			 .circle_G_radius =0.1f,
			 .G_l3bot = (lp) {0.6f,0.1f},
			 .G_l3top = (lp) {0.6f,0.45f},
			 .G_l4left = (lp) {0.45f,0.45f},
			 .G_l4right = (lp) {0.75f,0.45f},
			 .H_rightmid = (lp) {0.6f,0.5f},
			 .I_topright = (lp) {0.3f,1.f},
			 .I_bottomright = (lp) {0.3f,0.f},
			 .I_topbar = (lp) {0.15f,1.f},
			 .I_bottombar = (lp) {0.15f,0.f},
			 .J_topright = (lp) {0.6f,1.f},
			 .J_bottombar = (lp) {0.60f,0.3f},
			 .J_circle = (lp) {0.3f,0.3f},
			 .J_circle_radius = 0.3f,
			 .Q1 = (lp) {0.7f,0.3f},
			 .S_circle1 = (lp) {0.25f,0.75f},
			 .S_circle2 = (lp) {0.25f,0.25f},
			 .S_circle_radius = 0.25f,
			 .S_l1right = (lp) {0.25f,1.f},
			 .S_l2right = (lp) {0.25f,0.f},
			 .T_topbar = (lp) {0.30f,1.f},
			 .TVY_bottombar = (lp) {0.30f,0.f},
			 .U_l1bot = (lp){0.0f,0.30f},
			 .U_l2bot = (lp){0.6f,0.30f},
			 .U_circle1 = (lp) {0.3f,0.3f},
			 .U_circle_radius = 0.3f,
			 .W_p1 = (lp) {0.25f,0.f},
			 .W_p2 = (lp) {0.75f,0.f},
			 .Y_center = (lp) {0.3f,0.5f},
			 .carat_l1 = (lp) {0.f,0.8f},
			 .carat_l2 = (lp) {0.25f,1.f},
			 .carat_l3 = (lp) {0.50f,0.8f},
			 .backquote_l1 = (lp) {0.25f,0.75f},
			 .lower_circle = (lp) {0.25f,0.25f},
			 .lower_circle_radius = 0.25f,
			 .a_l1 = (lp) {0.5f,0.45f},
			 .c_l1 = (lp) {0.25f,0.5f},
			 .c_l2 = (lp) {0.25f,0.0f},
			 .e_l1 = (lp) {0.f,0.25f},
			 .e_l2 = (lp) {0.5f,0.25f},
			 .e_l3 = (lp) {0.25f,0.f},
			 .e_l4 = (lp) {0.35f,0.f},
			 .ft_bartop = {0.1f,0.825f},
			 .ft_barbottom = {0.1f,0.f},
			 .f_l1 = (lp) {0.f,0.5f},
			 .f_l2 = (lp) {0.2f,0.5f},
			 .f_circle = (lp) {0.275f,0.825f},
			 .f_circle_radius = 0.175f,
			 .f_l3 = (lp) {0.45f,0.825f},
			 .g_l1 = (lp) {0.5f,0.45f},
			 .g_l2 = (lp) {0.5f,-0.25f},
			 .g_circle = (lp) {0.35f,-0.25f},
			 .g_circle_radius = 0.15f,
			 .h_l1 = (lp) {0.5f,0.25f},
                         .i_bottom = (lp) {0.04f,0.0f},
                         .i_top = (lp) {0.04f,0.5f},
                         .i_dot = (lp) {0.04f,0.65f},
			 .j_l1 = (lp) {0.3f,0.5f},
			 .j_l2 = (lp) {0.3f,-0.25f},
			 .j_circle = (lp) {0.15f,-0.25f},
			 .j_circle_radius = 0.15f,
                         .j_dot = (lp) {0.3f,0.65f},
			 .k_l1 = (lp) {0.f,0.25f},
			 .k_l2 = (lp) {0.25f,0.50f},
			 .k_l3 = (lp) {0.30f,0.f},
			 .mnpr_topleft = (lp) {0.f,0.45f},
			 .m_l1 = (lp) {0.30f,0.35f},
			 .m_l2 = (lp) {0.30f,0.f},
			 .m_l3 = (lp) {0.6f,0.35f},
			 .m_l4 = (lp) {0.6f,0.f},
			 .m_circle1 = (lp) {0.15,0.35},
			 .m_circle_radius = 0.15,
			 .m_circle2 = (lp) {0.45,0.35},
			 .nr_circle1 = (lp) {0.25,0.25},
			 .nr_circle_radius = 0.25,
			 .nr_l1 = (lp) {0.5f,0.25f},
			 .n_l2 = (lp) {0.5f,0.0f},
			 .p_bottombar = (lp) {0.f,-0.40f},
			 .q_circle = (lp) {0.65f,-0.25f},
			 .q_circle_radius = 0.15f,
			 .q_l3 = (lp) {0.80f,-0.5f},
			 .s_circle1 = (lp) {0.125f,0.375f},
			 .s_circle2 = (lp) {0.125f,0.125f},
			 .s_circle_radius = 0.125f,
			 .s_l1left = (lp) {0.125f,0.5f},
			 .s_l1right = (lp) {0.25f,0.5f},
			 .s_l2right = (lp) {0.125f,0.f},
			 .s_extra_spot = (lp) {0.125f,0.25f},
			 .t_l1 = (lp) {0.f,0.57f},
			 .t_l2 = (lp) {0.2f,0.57f},
			 .uvwxyz_topleft = (lp) {0.f,0.5f},
			 .u_p1 = (lp) {0.f,0.25f},
			 .v_p1 = (lp) {0.25f,0.f},
			 .w_p1 = (lp) {0.15f,0.0f},
			 .w_p2 = (lp) {0.30f,0.5f},
			 .w_p3 = (lp) {0.45f,0.f},
			 .w_p4 = (lp) {0.6f,0.5f},
			 .y_p1 = (lp) {0.0f,-0.5f},
			 .close_brace_circle_1 = (lp) {0.30f,0.65f},
			 .close_brace_circle_2 = (lp) {0.30f,0.35f},
			 .close_brace_circle_3 = (lp) {0.0f,0.85f},
			 .close_brace_circle_4 = (lp) {0.0f,0.15f},
			 .close_brace_inside_radius = 0.15f,
			 .close_brace_outside_radius = 0.15f,
			 .close_brace_midpoint = (lp) {0.30f,0.50f},
			 .braces_l1_p1 = (lp) {0.15f,0.85f},
			 .braces_l1_p2 = (lp) {0.15f,0.65f},
			 .braces_l2_p1 = (lp) {0.15f,0.35f},
			 .braces_l2_p2 = (lp) {0.15f,0.15f},
			 .open_brace_circle_1 = (lp)  {0.0f,0.65f},
			 .open_brace_circle_2 = (lp) {0.0f,0.35f},
			 .open_brace_circle_3 = (lp) {0.30f,0.85f},
			 .open_brace_circle_4 = (lp) {0.30f,0.15f},
			 .open_brace_inside_radius = 0.15f,
			 .open_brace_outside_radius = 0.15f,
			 .open_brace_toppoint = (lp) {0.30f,1.f},
			 .open_brace_midpoint = (lp) {0.00f,0.50f},
			 .open_brace_bottompoint = (lp) {0.30f,0.0f},
			 
			 .vertical_bar_p1 = (lp) {0.f,1.f},
			 .vertical_bar_p2 = (lp) {0.f,0.55f},
			 .vertical_bar_p3 = (lp) {0.f,0.45f},
			 .vertical_bar_p4 = (lp) {0.f,0.f},
			 
			 .tilde_circle1 = (lp) {0.15f,0.85f},
			 .tilde_circle2 = (lp) {0.3f,1.1106025},  //guess with epsilon
			 .tilde_circle_radius = 0.15f,
			 
			 .del_p1 = (lp) {1.f,1.f},
			 .del_p2 = (lp) {0.f,1.f},
			 .del_p3 = (lp) {1.f,0.f},
			 .del_p4 = (lp) {0.f,0.f},
			 .del_p5 = (lp) {0.74455555f,0.25555555f},
			 .del_p6 = (lp) {0.25544445f,0.74444445f},
			 
                         }; // ex= 0.04    ey=0.04
return lnp;
};



static int ls_NUL(lcontext *lc) {ls_NONOP(lc);}
static int ls_SOH(lcontext *lc) {ls_NONOP(lc);}
static int ls_STX(lcontext *lc) {ls_NONOP(lc);}
static int ls_ETX(lcontext *lc) {ls_NONOP(lc);}
static int ls_EOT(lcontext *lc) {ls_NONOP(lc);}
static int ls_ENQ(lcontext *lc) {ls_NONOP(lc);}
static int ls_ACK(lcontext *lc) {ls_NONOP(lc);}
static int ls_BEL(lcontext *lc) {ls_NONOP(lc);}   
static int ls_BS(lcontext *lc) {ls_NONOP(lc);}
static int ls_TAB(lcontext *lc) {ls_NONOP(lc);}
static int ls_LF(lcontext *lc) {ls_NONOP(lc);}
static int ls_VT(lcontext *lc) {ls_NONOP(lc);}
static int ls_FF(lcontext *lc) {ls_NONOP(lc);}
static int ls_CR(lcontext *lc) {ls_NONOP(lc);}
static int ls_SO(lcontext *lc) {ls_NONOP(lc);}
static int ls_SI(lcontext *lc) {ls_NONOP(lc);}
static int ls_DLE(lcontext *lc) {ls_NONOP(lc);}
static int ls_DC1(lcontext *lc) {ls_NONOP(lc);}
static int ls_DC2(lcontext *lc) {ls_NONOP(lc);}
static int ls_DC3(lcontext *lc) {ls_NONOP(lc);}
static int ls_DC4(lcontext *lc) {ls_NONOP(lc);}
static int ls_NAK(lcontext *lc) {ls_NONOP(lc);}
static int ls_SYN(lcontext *lc) {ls_NONOP(lc);}
static int ls_ETB(lcontext *lc) {ls_NONOP(lc);}
static int ls_CAN(lcontext *lc) {ls_NONOP(lc);}
static int ls_EM(lcontext *lc) {ls_NONOP(lc);}
static int ls_SUB(lcontext *lc) {ls_NONOP(lc);}
static int ls_ESC(lcontext *lc) {ls_NONOP(lc);}
static int ls_FS(lcontext *lc) {ls_NONOP(lc);}
static int ls_GS(lcontext *lc) {ls_NONOP(lc);}
static int ls_RS(lcontext *lc) {ls_NONOP(lc);}
static int ls_US(lcontext *lc) {ls_NONOP(lc);}
   
static int ls_Space(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = {1.f,lnp->bottomright.x}};
return (lcan_add_boundary(lc));
}

static int ls_exclamation(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = (lp) {lnp->min_glyph_width,lnp->topright.y}};
return (lcan_add_boundary(lc));
}

   


static int ls_double_quote(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->double_quote_l2_p1};
return (lcan_add_boundary(lc));
}



static int ls_hash(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->hash_l1_p2.x, lnp->hash_l3_p2.y} };
return (lcan_add_boundary(lc));
}



static int ls_dollar(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->dollar_circle1.x +lnp->dollar_circle_radius, lnp->midsize_top.y} };
return (lcan_add_boundary(lc));
}



static int ls_percent(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_andpersand(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->dollar_circle1.x +lnp->dollar_circle_radius, lnp->midsize_top.y} };
return (lcan_add_boundary(lc));
}



static int ls_single_quote(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = (lp) {lnp->min_glyph_width,lnp->topright.y}};
return (lcan_add_boundary(lc));
}



static int ls_open_paren(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->openparen_upper_right};
return (lcan_add_boundary(lc));
}



static int ls_close_paren(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->openparen_upper_right};
return (lcan_add_boundary(lc));
}



static int ls_star(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_plus(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->plus_l2_p1.x,lnp->plus_l1_p1.y} };
return (lcan_add_boundary(lc));
}



static int ls_comma(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->comma_l1_p1, .upper_right=lnp->comma_l1_p2};
return (lcan_add_boundary(lc));
}



static int ls_minus(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->minus_l1_p1};
return (lcan_add_boundary(lc));
}



static int ls_dot(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = (lp) {lnp->min_glyph_width,lnp->circle_C.y}};
return (lcan_add_boundary(lc));
}



static int ls_slash(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_0(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_1(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->I_topright};
return (lcan_add_boundary(lc));
}



static int ls_2(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_3(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_4(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_5(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_6(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_7(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_8(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_9(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_colon(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = (lp) {lnp->min_glyph_width,lnp->colon_p1.y}};
return (lcan_add_boundary(lc));
}



static int ls_semicolon(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = lnp->semicolon_p1};
return (lcan_add_boundary(lc));
}



static int ls_less_than(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->lessthan_p1};
return (lcan_add_boundary(lc));
}



static int ls_equals(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->equals_l1_p2};
return (lcan_add_boundary(lc));
}



static int ls_greater_than(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->lessthan_p1};
return (lcan_add_boundary(lc));
}



static int ls_question_mark(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_at(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_A(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right={1.f,lnp->topright.x}};
return (lcan_add_boundary(lc));
}



static int ls_B(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright_B};
return (lcan_add_boundary(lc));
}



static int ls_C(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_D(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_E(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_F(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_G(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right = (lp) {.x=lnp->G_l4right.x,.y=lnp->topright.y} };
return (lcan_add_boundary(lc));
}



static int ls_H(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_I(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->I_topright};
return (lcan_add_boundary(lc));
}



static int ls_J(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_K(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_L(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_M(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_N(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_O(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_P(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {.x=lnp->circle_BPR_radius,.y=lnp->topright.y}};
return (lcan_add_boundary(lc));
}



static int ls_Q(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_R(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {.x=lnp->circle_BPR_radius,.y=lnp->topright.y}};
return (lcan_add_boundary(lc));
}



static int ls_S(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_T(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_U(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_V(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_W(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_X(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_Y(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_Z(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_open_bracket(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->I_topright};
return (lcan_add_boundary(lc));
}



static int ls_backslash(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_close_bracket(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->I_topright};
return (lcan_add_boundary(lc));
}



static int ls_carat(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_underscore(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}

static int ls_backquote(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->backquote_l1};
return (lcan_add_boundary(lc));
}


static int ls_a(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_b(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_c(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->c_l1};
return (lcan_add_boundary(lc));
}



static int ls_d(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_e(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_f(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->f_l3.x, lnp->halfsize_top.y} };
return (lcan_add_boundary(lc));
}



static int ls_g(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left= (lp) {.x=lnp->bottomleft.x, .y=lnp->g_circle.y - lnp->g_circle_radius} , .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_h(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->halfsize_top};
return (lcan_add_boundary(lc));
}



static int ls_i(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = {lnp->min_glyph_width,lnp->i_dot.y}};
return (lcan_add_boundary(lc));
}



static int ls_j(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left= (lp) {.x=lnp->bottomleft.x, .y=lnp->j_circle.y - lnp->j_circle_radius} , .upper_right=lnp->j_dot};
return (lcan_add_boundary(lc));
}



static int ls_k(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->k_l3.x,lnp->halfsize_top.y} };
return (lcan_add_boundary(lc));
}



static int ls_l(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left={0.f,0.f}, .upper_right = {lnp->min_glyph_width,lnp->halfsize_top.y}};
return (lcan_add_boundary(lc));
}



static int ls_m(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->midsize_top.x,lnp->circle_C.y} };
return (lcan_add_boundary(lc));
}



static int ls_n(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->halfsize_top.x,lnp->circle_C.y} };
return (lcan_add_boundary(lc));
}



static int ls_o(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->halfsize_top.x,lnp->circle_C.y} };
return (lcan_add_boundary(lc));
}



static int ls_p(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left= lnp->p_bottombar , .upper_right= (lp) {lnp->halfsize_top.x,lnp->circle_C.y} };
return (lcan_add_boundary(lc));
}



static int ls_q(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left= lnp->p_bottombar , .upper_right= (lp) {lnp->q_l3.x,lnp->circle_C.y} }; 
return (lcan_add_boundary(lc));
}



static int ls_r(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_s(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= lnp->s_l1right};
return (lcan_add_boundary(lc));
}



static int ls_t(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) {lnp->t_l2.x, lnp->ft_bartop.y} };
return (lcan_add_boundary(lc));
}



static int ls_u(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_v(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_w(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->w_p4};
return (lcan_add_boundary(lc));
}



static int ls_x(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_y(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->y_p1, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_z(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->circle_C};
return (lcan_add_boundary(lc));
}



static int ls_open_brace(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= lnp->open_brace_toppoint };
return (lcan_add_boundary(lc));
}



static int ls_vertical_bar(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->vertical_bar_p4, .upper_right = {lnp->min_glyph_width,lnp->halfsize_top.y}};
return (lcan_add_boundary(lc));
}



static int ls_close_brace(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right= (lp) { lnp->close_brace_midpoint.x, lnp->midsize_top.y} };
return (lcan_add_boundary(lc));
}



static int ls_tilde(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}



static int ls_DEL(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->topright};
return (lcan_add_boundary(lc));
}



static int ls_NOT_ASCII(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
lc->next_character_boundary = (lr) { .lower_left=lnp->bottomleft, .upper_right=lnp->midsize_top};
return (lcan_add_boundary(lc));
}








/* level 1 partial gliphs */




static inline void l1_dot(lneeded_points *lnp) {
lg_circle(10,lnp->line_width*0.5f,lnp->dot);
//lg_wide_line(lnp->dot,lnp->dot,lnp->line_width);
}




static inline void l1_cap(lneeded_points *lnp,lp p) {
lg_circle(10,lnp->line_width*0.5f,p);
}

static inline void l1_line(lneeded_points *lnp,lp a,lp b) { // cap b
l1_cap(lnp,b);
lg_wide_line(a,b,lnp->line_width);
}

static inline void l1_capline(lneeded_points *lnp,lp a,lp b) {
l1_cap(lnp,a);
l1_cap(lnp,b);
lg_wide_line(a,b,lnp->line_width);
}

static inline void l1_leftline(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->bottomleft);
}

static inline void l1_rightline(lneeded_points *lnp) {
l1_capline(lnp,lnp->topright,lnp->bottomright);
}

static inline void l1_midline(lneeded_points *lnp) {
l1_capline(lnp,lnp->midsize_top,lnp->midsize_bottom);
}

static inline void l1_eftop(lneeded_points *lnp) {
l1_line(lnp,lnp->topleft,lnp->EF_topright);
l1_line(lnp,lnp->EFH_midleft,lnp->EF_midright);
}



static inline int l1_lower_circle(lneeded_points *lnp) {
lg_wide_arc(10*4,lnp->lower_circle_radius,lnp->lower_circle,0.f,365.f,lnp->line_width,0,0);
return 0;
}


int ld_NUL(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_SOH(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_STX(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_ETX(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_EOT(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_ENQ(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_ACK(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_BEL(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_BS(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_TAB(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_LF(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_VT(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_FF(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_CR(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_SO(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_SI(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_DLE(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_DC1(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_DC2(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_DC3(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_DC4(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_NAK(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_SYN(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_ETB(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_CAN(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_EM(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_SUB(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_ESC(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_FS(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_GS(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_RS(lneeded_points *lnp) {ld_NONOP(lnp);}
int ld_US(lneeded_points *lnp) {ld_NONOP(lnp);}



int ld_Space(lneeded_points *lnp) { /* nothing to draw */
return 1;
}
  
  
int ld_exclamation(lneeded_points *lnp) {
lg_circle(10,lnp->line_width*0.5f,lnp->top_exclamation);
lg_circle(10,lnp->line_width*0.5f,lnp->bottom_exclamation);
lg_wide_line(lnp->top_exclamation,lnp->bottom_exclamation,lnp->line_width);
l1_dot(lnp);
return 1;
}




static int ld_double_quote(lneeded_points *lnp) {
l1_capline(lnp,lnp->double_quote_l1_p1,lnp->double_quote_l1_p2);
l1_capline(lnp,lnp->double_quote_l2_p1,lnp->double_quote_l2_p2);
return 1;
}



static int ld_hash(lneeded_points *lnp) {
l1_capline(lnp,lnp->hash_l1_p1,lnp->hash_l1_p2);
l1_capline(lnp,lnp->hash_l2_p1,lnp->hash_l2_p2);
l1_capline(lnp,lnp->hash_l3_p1,lnp->hash_l3_p2);
l1_capline(lnp,lnp->hash_l4_p1,lnp->hash_l4_p2);

return 1;
}



static int ld_dollar(lneeded_points *lnp) {
l1_capline(lnp,lnp->dollar_top,lnp->dollar_bottom);

lg_wide_arc(10*2,lnp->dollar_circle_radius,lnp->dollar_circle1,60.f,210.f,lnp->line_width,0,1);

lg_wide_arc(10*2,lnp->dollar_circle_radius,lnp->dollar_circle2,240.f,210.f,lnp->line_width,1,0);


return 1;
}



static int ld_percent(lneeded_points *lnp) {
l1_capline(lnp,lnp->bottomleft,lnp->topright);
lg_wide_arc(10*2,lnp->percent_circle_radius,lnp->percent_circle1,0.f,365.f,lnp->line_width,0,0);
lg_wide_arc(10*2,lnp->percent_circle_radius,lnp->percent_circle2,0.f,365.f,lnp->line_width,0,0);

return 1;
}



static int ld_andpersand(lneeded_points *lnp) { /* basically the dollar */

l1_capline(lnp,lnp->dollar_top,lnp->dollar_bottom);

lg_wide_arc(10*2,lnp->dollar_circle_radius,lnp->dollar_circle1,60.f,210.f,lnp->line_width,1,1);

lg_wide_arc(10*2,lnp->dollar_circle_radius,lnp->dollar_circle2,90.f,210.f,lnp->line_width,0,1);

l1_line(lnp,(lp) {lnp->dollar_circle2.x, lnp->circle_C.y}, (lp) { lnp->dollar_circle2.x + lnp->dollar_circle_radius*0.3f,lnp->circle_C.y});

return 1;
}



static int ld_single_quote(lneeded_points *lnp) {
l1_capline(lnp,lnp->single_quote_l1_p1,lnp->single_quote_l1_p2);

return 1;
}



static int ld_open_paren(lneeded_points *lnp) {
lg_wide_arc(10*4,lnp->openparen_circle_radius,lnp->openparen_circle1,150.f,60.f,lnp->line_width,1,1);
return 1;
}



static int ld_close_paren(lneeded_points *lnp) {
lg_wide_arc(10*4,lnp->closeparen_circle_radius,lnp->closeparen_circle1,330.f,60.f,lnp->line_width,1,1);
return 1;
}



static int ld_star(lneeded_points *lnp) {
l1_capline(lnp,lnp->star_l1_p1,lnp->star_l1_p2);
l1_capline(lnp,lnp->star_l2_p1,lnp->star_l2_p2);
l1_capline(lnp,lnp->star_l3_p1,lnp->star_l3_p2);
return 1;
}



static int ld_plus(lneeded_points *lnp) {
l1_capline(lnp,lnp->plus_l1_p1,lnp->plus_l1_p2);
l1_capline(lnp,lnp->plus_l2_p1,lnp->plus_l2_p2);

return 1;
}



static int ld_comma(lneeded_points *lnp) {
l1_capline(lnp,lnp->comma_l1_p1,lnp->comma_l1_p2);

return 1;
}



static int ld_minus(lneeded_points *lnp) {
l1_capline(lnp,lnp->minus_l1_p1,lnp->minus_l1_p2);

return 1;
}



static int ld_dot(lneeded_points *lnp) {
l1_dot(lnp);
return 1;
}



static int ld_slash(lneeded_points *lnp) {
l1_capline(lnp,lnp->bottomleft,lnp->midsize_top);
return 1;
}



static int ld_0(lneeded_points *lnp) {
lg_wide_arc(10*4,lnp->circle_C_radius,lnp->circle_C,0.f,365.f,lnp->line_width,0,0);

l1_cap(lnp,lnp->circle_C); // add a dot

return 1;
}



static int ld_1(lneeded_points *lnp) {
lg_wide_line(lnp->I_topbar,lnp->I_bottombar,lnp->line_width);

l1_capline(lnp,lnp->one_p1,lnp->one_p2);
l1_capline(lnp,lnp->bottomleft,lnp->I_bottomright);

return 1;
}



static int ld_2(lneeded_points *lnp) {
lg_wide_arc(10*3,lnp->two_circle_radius,lnp->two_circle,313.f,250.f,lnp->line_width,1,1);
l1_line(lnp,lnp->two_circle_l1_p1,lnp->two_circle_l1_p2);
l1_line(lnp,lnp->two_circle_l1_p2,lnp->midsize_bottom);

return 1;
}



static int ld_3(lneeded_points *lnp) {

lg_wide_arc(10*2,lnp->S_circle_radius,lnp->S_circle1,270.f,210.f,lnp->line_width,0,1);

lg_wide_arc(10*2,lnp->S_circle_radius,lnp->S_circle2,240.f,210.f,lnp->line_width,1,1);

return 1;
}



static int ld_4(lneeded_points *lnp) {
l1_capline(lnp,lnp->four_l1_p1,lnp->four_l1_p2);
l1_line(lnp,lnp->four_l1_p2,lnp->four_l1_p3);
l1_line(lnp,lnp->four_l1_p3,lnp->four_l1_p4);
return 1;
}



static int ld_5(lneeded_points *lnp) {
l1_capline(lnp,lnp->five_l1_p1,lnp->five_l1_p2);
l1_line(lnp,lnp->five_l1_p2,lnp->five_l1_p3);
l1_line(lnp,lnp->five_l1_p3,lnp->five_l1_p4);
lg_wide_arc(10*2,lnp->five_circle_radius,lnp->five_circle,240.f,210.f,lnp->line_width,1,0);

return 1;
}



static int ld_6(lneeded_points *lnp) {
lg_wide_arc(10*3,lnp->five_circle_radius,lnp->five_circle,240.f,365.f,lnp->line_width,0,0);
lg_wide_line(lnp->six_p1,lnp->six_p2,lnp->line_width);
lg_wide_arc(10*4,lnp->six_circle2_radius,lnp->six_circle2,90.f,90.f,lnp->line_width,1,1);

return 1;
}



static int ld_7(lneeded_points *lnp) {
l1_capline(lnp,lnp->seven_p1,lnp->seven_p2);
l1_line(lnp,lnp->seven_p2,lnp->seven_p3);

return 1;
}



static int ld_8(lneeded_points *lnp) {
lg_wide_arc(10*4,lnp->five_circle_radius,lnp->five_circle,0.f,365.f,lnp->line_width,0,0);
lg_wide_arc(10*4,lnp->eight_circle_radius,lnp->eight_circle2,0.f,365.f,lnp->line_width,0,0);
return 1;
}



static int ld_9(lneeded_points *lnp) {
lg_wide_arc(10*4,lnp->eight_circle_radius,lnp->eight_circle2,240.f,365.f,lnp->line_width,0,0);
lg_wide_line(lnp->nine_p1,lnp->nine_p2,lnp->line_width);
lg_wide_arc(10*4,lnp->nine_circle2_radius,lnp->nine_circle2,270.f,90.f,lnp->line_width,1,1);

return 1;
}



static int ld_colon(lneeded_points *lnp) {
l1_cap(lnp,lnp->colon_p1);
l1_cap(lnp,lnp->colon_p2);
return 1;
}



static int ld_semicolon(lneeded_points *lnp) {
l1_cap(lnp,lnp->semicolon_p1);
l1_capline(lnp,lnp->semicolon_p2,lnp->semicolon_p3);
return 1;
}



static int ld_less_than(lneeded_points *lnp) {
l1_capline(lnp,lnp->lessthan_p1,lnp->lessthan_p2);
l1_line(lnp,lnp->lessthan_p2,lnp->lessthan_p3);

return 1;
}



static int ld_equals(lneeded_points *lnp) {
l1_capline(lnp,lnp->equals_l1_p1,lnp->equals_l1_p2);
l1_capline(lnp,lnp->equals_l2_p1,lnp->equals_l2_p2);
return 1;
}



static int ld_greater_than(lneeded_points *lnp) {
l1_capline(lnp,lnp->greaterthan_p1,lnp->greaterthan_p2);
l1_line(lnp,lnp->greaterthan_p2,lnp->greaterthan_p3);

return 1;
}



static int ld_question_mark(lneeded_points *lnp) {


lg_wide_arc(10*4,lnp->eight_circle_radius,lnp->eight_circle2,270.f,225.f,lnp->line_width,1,1);
l1_line(lnp,lnp->question_mark_p2,lnp->question_mark_p1);
l1_cap(lnp,lnp->question_mark_dot);
return 1;
}



static int ld_at(lneeded_points *lnp) {
lg_wide_arc(10*4,lnp->at_circle1_radius,lnp->at_circle1,0.f,365.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->at_l1_p1,lnp->at_l1_p2);
lg_wide_arc(10*3,lnp->at_circle2_radius,lnp->at_circle2,270.f,90.f,lnp->line_width,1,1);
lg_wide_arc(10*8,lnp->at_circle3_radius,lnp->at_circle1,0.f,315.f,lnp->line_width,1,1);

return 1;
}



static int ld_A(lneeded_points *lnp) {
l1_cap(lnp,lnp->bottomleft);
l1_cap(lnp,lnp->topmid);
lg_wide_line(lnp->topmid,lnp->bottomleft,lnp->line_width);


l1_cap(lnp,lnp->bottomright);
lg_wide_line(lnp->topmid,lnp->bottomright,lnp->line_width);

lg_wide_line(lnp->mid_A_left,lnp->mid_A_right,lnp->line_width);

return 1;
}



static int ld_B(lneeded_points *lnp) {
l1_leftline(lnp);
lg_wide_arc(10*2,lnp->circle_BPR_radius,lnp->circle_B_top,270.f,180.f,lnp->line_width,0,0);

lg_wide_arc(10*2,lnp->circle_BPR_radius,lnp->circle_B_bottom,270.f,180.f,lnp->line_width,0,0);

return 1;
}



static int ld_C(lneeded_points *lnp) {
lg_wide_arc(10*3,lnp->circle_C_radius,lnp->circle_C,90.f,180.f,lnp->line_width,0,0);
l1_cap(lnp,lnp->topmid);
l1_cap(lnp,lnp->bottommid);
return 1;
}



static int ld_D(lneeded_points *lnp) {
l1_leftline(lnp);

lg_wide_arc(10*3,lnp->circle_C_radius,lnp->circle_D,270.f,180.f,lnp->line_width,0,0);

return 1;
}



static int ld_E(lneeded_points *lnp) {
l1_leftline(lnp);
l1_eftop(lnp);

l1_line(lnp,lnp->bottomleft,lnp->E_bottomright);
return 1;
}



static int ld_F(lneeded_points *lnp) {
l1_leftline(lnp);
l1_eftop(lnp);
return 1;
}



static int ld_G(lneeded_points *lnp) {
ld_C(lnp);
lg_wide_arc(10,lnp->circle_G_radius,lnp->circle_G,270.f,90.f,lnp->line_width,0,0);
lg_wide_line(lnp->G_l3bot,lnp->G_l3top,lnp->line_width);
l1_capline(lnp,lnp->G_l4left,lnp->G_l4right);

return 1;
}



static int ld_H(lneeded_points *lnp) {
l1_leftline(lnp);
l1_midline(lnp);
lg_wide_line(lnp->EFH_midleft,lnp->H_rightmid,lnp->line_width);
return 1;
}



static int ld_I(lneeded_points *lnp) {
lg_wide_line(lnp->I_topbar,lnp->I_bottombar,lnp->line_width);
l1_capline(lnp,lnp->topleft,lnp->I_topright);
l1_capline(lnp,lnp->bottomleft,lnp->I_bottomright);
return 1;
}



static int ld_J(lneeded_points *lnp) {
l1_cap(lnp,lnp->J_topright);
lg_wide_line(lnp->J_topright,lnp->J_bottombar,lnp->line_width);
lg_wide_arc(10*2,lnp->J_circle_radius,lnp->J_circle,180.f,180.f,lnp->line_width,0,0);
l1_cap(lnp, (lp) {.x=0.f,.y=lnp->J_circle.y});

return 1;
}



static int ld_K(lneeded_points *lnp) {
l1_leftline(lnp);        
l1_line(lnp,lnp->circle_D,lnp->midsize_top);
l1_line(lnp,lnp->circle_D,lnp->midsize_bottom);
return 1;
}



static int ld_L(lneeded_points *lnp) {
l1_leftline(lnp);
l1_line(lnp,lnp->bottomleft,lnp->halfsize_bottom);
return 1;
}



static int ld_M(lneeded_points *lnp) {
l1_leftline(lnp);
l1_line(lnp,lnp->topleft,lnp->halfsize_bottom);
l1_line(lnp,lnp->halfsize_bottom,lnp->topright);
l1_line(lnp,lnp->topright,lnp->bottomright);

return 1;
}



static int ld_N(lneeded_points *lnp) {
l1_leftline(lnp);
l1_line(lnp,lnp->topleft,lnp->midsize_bottom);
l1_line(lnp,lnp->midsize_bottom,lnp->midsize_top);

return 1;
}



static int ld_O(lneeded_points *lnp) {
lg_wide_arc(10*8,lnp->circle_C_radius,lnp->circle_C,0.f,365.f,lnp->line_width,0,0);
return 1;
}



static int ld_P(lneeded_points *lnp) {
l1_leftline(lnp);
lg_wide_arc(10*2,lnp->circle_BPR_radius,lnp->circle_B_top,270.f,180.f,lnp->line_width,0,0);

return 1;
}



static int ld_Q(lneeded_points *lnp) {
ld_O(lnp);
l1_capline(lnp,lnp->Q1,lnp->bottomright);
return 1;
}



static int ld_R(lneeded_points *lnp) {
ld_P(lnp);
l1_line(lnp,lnp->circle_D,(lp) {.x = lnp->circle_BPR_radius, .y =lnp->bottomright.y});
return 1;
}



static int ld_S(lneeded_points *lnp) {
l1_cap(lnp,lnp->halfsize_top);
lg_wide_line(lnp->halfsize_top,lnp->S_l1right,lnp->line_width);

lg_wide_arc(10*2,lnp->S_circle_radius,lnp->S_circle1,90.f,180.f,lnp->line_width,0,1);

lg_wide_arc(10*2,lnp->S_circle_radius,lnp->S_circle2,270.f,180.f,lnp->line_width,0,0);
l1_cap(lnp,lnp->bottomleft);
lg_wide_line(lnp->bottomleft,lnp->S_l2right,lnp->line_width);
return 1;
}



static int ld_T(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->midsize_top);
l1_line(lnp,lnp->T_topbar,lnp->TVY_bottombar);
return 1;
}



static int ld_U(lneeded_points *lnp) {
l1_line(lnp,lnp->U_l1bot,lnp->topleft);
lg_wide_arc(10*3,lnp->U_circle_radius,lnp->U_circle1,180.f,180.f,lnp->line_width,1,1);
l1_line(lnp,lnp->U_l2bot,lnp->midsize_top);
return 1;
}



static int ld_V(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->TVY_bottombar);
l1_line(lnp,lnp->TVY_bottombar,lnp->midsize_top);
return 1;
}



static int ld_W(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->W_p1);
l1_line(lnp,lnp->W_p1,lnp->halfsize_top);
l1_line(lnp,lnp->halfsize_top,lnp->W_p2);
l1_line(lnp,lnp->W_p2,lnp->topright);
return 1;
}



static int ld_X(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->midsize_bottom);
l1_capline(lnp,lnp->bottomleft,lnp->midsize_top);
return 1;
}



static int ld_Y(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->Y_center);
l1_line(lnp,lnp->Y_center,lnp->midsize_top);
l1_line(lnp,lnp->Y_center,lnp->TVY_bottombar);

return 1;
}



static int ld_Z(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->midsize_top);
l1_line(lnp,lnp->midsize_top,lnp->bottomleft);
l1_line(lnp,lnp->bottomleft,lnp->midsize_bottom);

return 1;
}



static int ld_open_bracket(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->bottomleft);
l1_line(lnp,lnp->topleft,lnp->I_topright);
l1_line(lnp,lnp->bottomleft,lnp->I_bottomright);

return 1;
}



static int ld_backslash(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->midsize_bottom);
return 1;
}



static int ld_close_bracket(lneeded_points *lnp) {
l1_capline(lnp,lnp->I_topright,lnp->I_bottomright);
l1_line(lnp,lnp->I_topright,lnp->topleft);
l1_line(lnp,lnp->I_bottomright,lnp->bottomleft);

return 1;
}



static int ld_carat(lneeded_points *lnp) {
l1_capline(lnp,lnp->carat_l1,lnp->carat_l2);
l1_line(lnp,lnp->carat_l2,lnp->carat_l3);
return 1;
}



static int ld_underscore(lneeded_points *lnp) {
l1_capline(lnp,lnp->bottomleft,lnp->midsize_bottom);

return 1;
}



static int ld_backquote(lneeded_points *lnp) {
l1_capline(lnp,lnp->topleft,lnp->backquote_l1);
return 1;
}

static int ld_a(lneeded_points *lnp) {
l1_lower_circle(lnp);
l1_capline(lnp,lnp->halfsize_bottom,lnp->a_l1);
return 1;
}



static int ld_b(lneeded_points *lnp) {
l1_leftline(lnp);
l1_lower_circle(lnp);
return 1;
}



static int ld_c(lneeded_points *lnp) {
lg_wide_arc(10*3,lnp->lower_circle_radius,lnp->lower_circle,90.f,180.f,lnp->line_width,0,0);
l1_cap(lnp,lnp->c_l1);
l1_cap(lnp,lnp->c_l2);

return 1;
}



static int ld_d(lneeded_points *lnp) {
l1_lower_circle(lnp);
l1_capline(lnp,lnp->halfsize_bottom,lnp->halfsize_top);

return 1;
}



static int ld_e(lneeded_points *lnp) {
lg_wide_arc(10*3,lnp->lower_circle_radius,lnp->lower_circle,0.f,270.f,lnp->line_width,0,0);
lg_wide_line(lnp->e_l1,lnp->e_l2,lnp->line_width);
l1_capline(lnp,lnp->e_l3,lnp->e_l4);
l1_cap(lnp,lnp->e_l2);
return 1;
}



static int ld_f(lneeded_points *lnp) {
l1_capline(lnp,lnp->ft_bartop,lnp->ft_barbottom);
l1_capline(lnp,lnp->f_l1,lnp->f_l2);
lg_wide_arc(10*2,lnp->f_circle_radius,lnp->f_circle,0.f,180.f,lnp->line_width,0,0);
l1_cap(lnp,lnp->f_l3);
return 1;
}



static int ld_g(lneeded_points *lnp) {
l1_lower_circle(lnp);
l1_capline(lnp,lnp->g_l1,lnp->g_l2);
lg_wide_arc(10*2,lnp->g_circle_radius,lnp->g_circle,180.f,180.f,lnp->line_width,1,0);

return 1;
}



static int ld_h(lneeded_points *lnp) {
l1_leftline(lnp);
lg_wide_arc(10*2,lnp->lower_circle_radius,lnp->lower_circle,0.f,180.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->halfsize_bottom,lnp->h_l1);
return 1;
}



static int ld_i(lneeded_points *lnp) {
l1_capline(lnp,lnp->i_top,lnp->i_bottom);
l1_cap(lnp,lnp->i_dot);
return 1;
}



static int ld_j(lneeded_points *lnp) {
l1_capline(lnp,lnp->j_l2,lnp->j_l1);
lg_wide_arc(10*2,lnp->j_circle_radius,lnp->j_circle,180.f,180.f,lnp->line_width,1,0);
l1_cap(lnp,lnp->j_dot);

return 1;
}



static int ld_k(lneeded_points *lnp) {
l1_leftline(lnp);
l1_line(lnp,lnp->k_l1,lnp->k_l2);
l1_line(lnp,lnp->k_l1,lnp->k_l3);
return 1;
}



static int ld_l(lneeded_points *lnp) {
l1_capline(lnp,lnp->bottomleft,(lp) {0,lnp->top_exclamation.y});

return 1;
}



static int ld_m(lneeded_points *lnp) {
l1_capline(lnp,lnp->mnpr_topleft,lnp->bottomleft);
lg_wide_arc(10*2,lnp->m_circle_radius,lnp->m_circle1,0.f,180.f,lnp->line_width,0,0);
lg_wide_arc(10*2,lnp->m_circle_radius,lnp->m_circle2,0.f,180.f,lnp->line_width,0,0);
l1_line(lnp,lnp->m_l1,lnp->m_l2);
l1_capline(lnp,lnp->m_l3,lnp->m_l4);
return 1;
}



static int ld_n(lneeded_points *lnp) {
l1_capline(lnp,lnp->mnpr_topleft,lnp->bottomleft);
lg_wide_arc(10*2,lnp->nr_circle_radius,lnp->nr_circle1,0.f,180.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->nr_l1,lnp->n_l2);
return 1;
}



static int ld_o(lneeded_points *lnp) {
l1_lower_circle(lnp);
return 1;
}



static int ld_p(lneeded_points *lnp) {
l1_lower_circle(lnp);
l1_capline(lnp,lnp->mnpr_topleft,lnp->p_bottombar);
return 1;
}



static int ld_q(lneeded_points *lnp) {
l1_lower_circle(lnp);
l1_capline(lnp,lnp->g_l1,lnp->g_l2);
lg_wide_arc(10*2,lnp->q_circle_radius,lnp->q_circle,180.f,180.f,lnp->line_width,0,1);

return 1;
}



static int ld_r(lneeded_points *lnp) {
l1_capline(lnp,lnp->mnpr_topleft,lnp->bottomleft);
lg_wide_arc(10*2,lnp->nr_circle_radius,lnp->nr_circle1,0.f,180.f,lnp->line_width,0,0);
l1_cap(lnp,lnp->nr_l1);

return 1;
}



static int ld_s(lneeded_points *lnp) {
l1_capline(lnp,lnp->s_l1left,lnp->s_l1right);

lg_wide_arc(10*2,lnp->s_circle_radius,lnp->s_circle1,90.f,180.f,lnp->line_width,0,0);

lg_wide_arc(10*2,lnp->s_circle_radius,lnp->s_circle2,270.f,180.f,lnp->line_width,0,0);
l1_cap(lnp,lnp->bottomleft);
lg_wide_line(lnp->bottomleft,lnp->s_l2right,lnp->line_width);
l1_cap(lnp,lnp->s_extra_spot);
return 1;
}



static int ld_t(lneeded_points *lnp) {
l1_capline(lnp,lnp->ft_bartop,lnp->ft_barbottom);
l1_capline(lnp,lnp->t_l1,lnp->t_l2);
return 1;
}



static int ld_u(lneeded_points *lnp) {
l1_capline(lnp,lnp->uvwxyz_topleft,lnp->u_p1);
lg_wide_arc(10*2,lnp->lower_circle_radius,lnp->lower_circle,180.f,180.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->circle_C,lnp->halfsize_bottom);
return 1;
}



static int ld_v(lneeded_points *lnp) {
l1_capline(lnp,lnp->uvwxyz_topleft,lnp->v_p1);
l1_line(lnp,lnp->v_p1,lnp->circle_C);

return 1;
}



static int ld_w(lneeded_points *lnp) {
l1_capline(lnp,lnp->uvwxyz_topleft,lnp->w_p1);
l1_line(lnp,lnp->w_p1,lnp->w_p2);
l1_line(lnp,lnp->w_p2,lnp->w_p3);
l1_line(lnp,lnp->w_p3,lnp->w_p4);

return 1;
}



static int ld_x(lneeded_points *lnp) {
l1_capline(lnp,lnp->uvwxyz_topleft,lnp->halfsize_bottom);
l1_capline(lnp,lnp->bottomleft,lnp->circle_C);
return 1;
}



static int ld_y(lneeded_points *lnp) {
l1_capline(lnp,lnp->uvwxyz_topleft,lnp->v_p1);
l1_capline(lnp,lnp->y_p1,lnp->circle_C);
return 1;
}



static int ld_z(lneeded_points *lnp) {
l1_capline(lnp,lnp->uvwxyz_topleft,lnp->circle_C);
l1_line(lnp,lnp->circle_C,lnp->bottomleft);
l1_line(lnp,lnp->bottomleft,lnp->halfsize_bottom);
return 1;
}



static int ld_open_brace(lneeded_points *lnp) {
l1_cap(lnp,lnp->open_brace_toppoint);
lg_wide_arc(10,lnp->open_brace_outside_radius,lnp->open_brace_circle_3,90.f,90.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->braces_l1_p1,lnp->braces_l1_p2);
l1_cap(lnp,lnp->open_brace_midpoint);
lg_wide_arc(10,lnp->open_brace_inside_radius,lnp->open_brace_circle_1,270.f,90.f,lnp->line_width,0,0);

lg_wide_arc(10,lnp->open_brace_inside_radius,lnp->open_brace_circle_2,0.f,90.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->braces_l2_p1,lnp->braces_l2_p2);

l1_cap(lnp,lnp->open_brace_bottompoint);
lg_wide_arc(10,lnp->open_brace_outside_radius,lnp->open_brace_circle_4,180.f,90.f,lnp->line_width,0,0);

return 1;
}



static int ld_vertical_bar(lneeded_points *lnp) {
l1_capline(lnp,lnp->vertical_bar_p1,lnp->vertical_bar_p2);
l1_capline(lnp,lnp->vertical_bar_p3,lnp->vertical_bar_p4);
return 1;
}



static int ld_close_brace(lneeded_points *lnp) {
l1_cap(lnp,lnp->topleft);
lg_wide_arc(10,lnp->close_brace_outside_radius,lnp->close_brace_circle_3,0.f,90.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->braces_l1_p1,lnp->braces_l1_p2);
l1_cap(lnp,lnp->close_brace_midpoint);
lg_wide_arc(10,lnp->close_brace_inside_radius,lnp->close_brace_circle_1,180.f,90.f,lnp->line_width,0,0);

lg_wide_arc(10,lnp->close_brace_inside_radius,lnp->close_brace_circle_2,90.f,90.f,lnp->line_width,0,0);
l1_capline(lnp,lnp->braces_l2_p1,lnp->braces_l2_p2);

l1_cap(lnp,lnp->bottomleft);
lg_wide_arc(10,lnp->close_brace_outside_radius,lnp->close_brace_circle_4,270.f,90.f,lnp->line_width,0,0);

return 1;
}



static int ld_tilde(lneeded_points *lnp) {
lg_wide_arc(10*2,lnp->tilde_circle_radius,lnp->tilde_circle1,60.f,60.f,lnp->line_width,1,1);
lg_wide_arc(10*2,lnp->tilde_circle_radius,lnp->tilde_circle2,240.f,60.f,lnp->line_width,1,1);

return 1;
}



static int ld_DEL(lneeded_points *lnp) {
// logo
l1_line(lnp,lnp->del_p1,lnp->del_p2);
l1_line(lnp,lnp->del_p2,lnp->del_p3);
l1_line(lnp,lnp->del_p3,lnp->del_p4);
l1_line(lnp,lnp->del_p4,lnp->del_p5);
l1_line(lnp,lnp->del_p6,lnp->del_p1);

return 1;
}



static int ld_NOT_ASCII(lneeded_points *lnp) {

return 1;
}






/* heres the big interface */
linterface letters_ascii_interface = (linterface) {
 .lsize = {
      ls_NUL,
      ls_SOH,
      ls_STX,
      ls_ETX,
      ls_EOT,
      ls_ENQ,
      ls_ACK,
      ls_BEL,
      ls_BS,
      ls_TAB,
      ls_LF,
      ls_VT,
      ls_FF,
      ls_CR,
      ls_SO,
      ls_SI,
      ls_DLE,
      ls_DC1,
      ls_DC2,
      ls_DC3,
      ls_DC4,
      ls_NAK,
      ls_SYN,
      ls_ETB,
      ls_CAN,
      ls_EM,
      ls_SUB,
      ls_ESC,
      ls_FS,
      ls_GS,
      ls_RS,
      ls_US,
      ls_Space,
      ls_exclamation,
      ls_double_quote,
      ls_hash,
      ls_dollar,
      ls_percent,
      ls_andpersand,
      ls_single_quote,
      ls_open_paren,
      ls_close_paren,
      ls_star,
      ls_plus,
      ls_comma,
      ls_minus,
      ls_dot,
      ls_slash,
      ls_0,
      ls_1,
      ls_2,
      ls_3,
      ls_4,
      ls_5,
      ls_6,
      ls_7,
      ls_8,
      ls_9,
      ls_colon,
      ls_semicolon,
      ls_less_than,
      ls_equals,
      ls_greater_than,
      ls_question_mark,
      ls_at,
      ls_A,
      ls_B,
      ls_C,
      ls_D,
      ls_E,
      ls_F,
      ls_G,
      ls_H,
      ls_I,
      ls_J,
      ls_K,
      ls_L,
      ls_M,
      ls_N,
      ls_O,
      ls_P,
      ls_Q,
      ls_R,
      ls_S,
      ls_T,
      ls_U,
      ls_V,
      ls_W,
      ls_X,
      ls_Y,
      ls_Z,
      ls_open_bracket,
      ls_backslash,
      ls_close_bracket,
      ls_carat,
      ls_underscore,
      ls_backquote,
      ls_a,
      ls_b,
      ls_c,
      ls_d,
      ls_e,
      ls_f,
      ls_g,
      ls_h,
      ls_i,
      ls_j,
      ls_k,
      ls_l,
      ls_m,
      ls_n,
      ls_o,
      ls_p,
      ls_q,
      ls_r,
      ls_s,
      ls_t,
      ls_u,
      ls_v,
      ls_w,
      ls_x,
      ls_y,
      ls_z,
      ls_open_brace,
      ls_vertical_bar,
      ls_close_brace,
      ls_tilde,
      ls_DEL,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      ls_NOT_ASCII,
      },
 .ldraw = {
      ld_NUL,
      ld_SOH,
      ld_STX,
      ld_ETX,
      ld_EOT,
      ld_ENQ,
      ld_ACK,
      ld_BEL,
      ld_BS,
      ld_TAB,
      ld_LF,
      ld_VT,
      ld_FF,
      ld_CR,
      ld_SO,
      ld_SI,
      ld_DLE,
      ld_DC1,
      ld_DC2,
      ld_DC3,
      ld_DC4,
      ld_NAK,
      ld_SYN,
      ld_ETB,
      ld_CAN,
      ld_EM,
      ld_SUB,
      ld_ESC,
      ld_FS,
      ld_GS,
      ld_RS,
      ld_US,
      ld_Space,
      ld_exclamation,
      ld_double_quote,
      ld_hash,
      ld_dollar,
      ld_percent,
      ld_andpersand,
      ld_single_quote,
      ld_open_paren,
      ld_close_paren,
      ld_star,
      ld_plus,
      ld_comma,
      ld_minus,
      ld_dot,
      ld_slash,
      ld_0,
      ld_1,
      ld_2,
      ld_3,
      ld_4,
      ld_5,
      ld_6,
      ld_7,
      ld_8,
      ld_9,
      ld_colon,
      ld_semicolon,
      ld_less_than,
      ld_equals,
      ld_greater_than,
      ld_question_mark,
      ld_at,
      ld_A,
      ld_B,
      ld_C,
      ld_D,
      ld_E,
      ld_F,
      ld_G,
      ld_H,
      ld_I,
      ld_J,
      ld_K,
      ld_L,
      ld_M,
      ld_N,
      ld_O,
      ld_P,
      ld_Q,
      ld_R,
      ld_S,
      ld_T,
      ld_U,
      ld_V,
      ld_W,
      ld_X,
      ld_Y,
      ld_Z,
      ld_open_bracket,
      ld_backslash,
      ld_close_bracket,
      ld_carat,
      ld_underscore,
      ld_backquote,
      ld_a,
      ld_b,
      ld_c,
      ld_d,
      ld_e,
      ld_f,
      ld_g,
      ld_h,
      ld_i,
      ld_j,
      ld_k,
      ld_l,
      ld_m,
      ld_n,
      ld_o,
      ld_p,
      ld_q,
      ld_r,
      ld_s,
      ld_t,
      ld_u,
      ld_v,
      ld_w,
      ld_x,
      ld_y,
      ld_z,
      ld_open_brace,
      ld_vertical_bar,
      ld_close_brace,
      ld_tilde,
      ld_DEL,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      ld_NOT_ASCII,
      }
      };
      



float lkern_ascii(lcontext *lc) {
lneeded_points *lnp = lc->needed_points;
if (!(lc->cursor)) {
  return 0.f;
  }
if (lc->next_character == '\0') {
  return lnp->standard_kern;
  }
if (lc->previous_character == '\0') {
  return lnp->standard_kern;
  }


if (lc->next_character == 'j') {		//					_j
  if (lc->previous_character == 'p') { //	pj
    return lnp->standard_kern-0.3f;
    }
  else if (lc->previous_character == 'y') { //	yj
    return lnp->standard_kern-0.25f;
    }
  else if (lc->previous_character_boundary.lower_left.y >= -0.25f) { // 	j can slip under
    return lnp->standard_kern-0.3f;         //  cj
    }
  }
else if (lc->previous_character == '\\') {      //					backslash
  if (lc->next_character == 'V') {   // 	\V
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 'T') { //	\T
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 'y') { // 	\y
    return lnp->standard_kern-0.20f;
    }
  else if (lc->next_character == 'v') { // 	\v
    return lnp->standard_kern-0.20f;
    }
  else if (lc->next_character == '\\') { // 	\\ backslash
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == 'f') { // 	\f
    return lnp->standard_kern-0.15f;
    }
  else if (lc->next_character == 't') { // 	\t
    return lnp->standard_kern-0.1f;
    }
  }
else if (lc->previous_character == 'L') {	//					L
  if (lc->next_character == 'V') {   // 	LV
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 'T') { //	LT
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 'y') { // 	Ly
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == 'v') { // 	Lv
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == '\\') { // 	L\
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == 'f') { // 	Lf
    return lnp->standard_kern-0.15f;
    }
  else if (lc->next_character == 't') { // 	Lt
    return lnp->standard_kern-0.1f;
    }
  }
/* \ slanting slope A */
else if (lc->previous_character == 'A') {	//					A
  if (lc->next_character == 'V') {   // 	AV
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 'y') { // 	Ay
    return lnp->standard_kern-0.20f;
    }
  else if (lc->next_character == 'v') { // 	Av
    return lnp->standard_kern-0.20f;
    }
  else if (lc->next_character == '\\') { // 	A backslash
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == 'f') { // 	Af
    return lnp->standard_kern-0.15f;
    }
  else if (lc->next_character == 't') { // 	At
    return lnp->standard_kern-0.1f;
    }       
  else if (lc->next_character == 'T') { //	AT
    return lnp->standard_kern-0.3f;
    }
  }
else if ((lc->previous_character == 'b')||	//					b
         (lc->previous_character == 'h')||	//					h
         (lc->previous_character == 'k')) {	//					k
  if (lc->next_character == 'V') {   // 	bV	hV	kV
    return lnp->standard_kern-0.15f;
    }
  else if (lc->next_character == '\\') { // 	b\	h\	k backslash
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 't') { // 	bt	ht	kt
    return lnp->standard_kern-0.1f;
    }
  else if (lc->next_character == 'T') { //	\T
    return lnp->standard_kern-0.3f;
    }
  }
else if (lc->previous_character == 't') {	//					t					
  if (lc->next_character == 'y') { //	ty 	
    return lnp->standard_kern-0.1f;
    }
  else if (lc->next_character == 'v') { //	tv 	
    return lnp->standard_kern-0.1f;
    }
  else if (lc->next_character == '\\') { // 	t\ backslash
    return lnp->standard_kern-0.1;
    }
  else if (lc->previous_character_boundary.upper_right.y <= 0.5f) {
    return lnp->standard_kern-0.1f;
    }    
  else if (lc->next_character == 'T') { //	\T
    return lnp->standard_kern-0.3f;
    }
  }
else if (lc->previous_character == 'j') {	//					j				
  if (lc->next_character == '\\') { // 			j\\ backslash
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == 'T') { //	        jT
    return lnp->standard_kern-0.3f;
    }
  }
else if (lc->previous_character == 'q') {	//					q
  if (lc->next_character == 'q') {	//		qq
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 'V') {   // 		qV
    return lnp->standard_kern-0.30f;
    }
  else if (lc->next_character == '\\') { // 		q\\ backslash
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == 't') { // 		qt
    return lnp->standard_kern-0.1f;
    }
  else if (lc->next_character == 'T') { // 		qT
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character_boundary.lower_left.y >= -0.25f) {
    return lnp->standard_kern-0.3f;
    }
  }
else if ((lc->previous_character == 'v')	//					v
       ||(lc->previous_character == 'y')) {	//					y
  if (lc->next_character == 'A') {   // 	vA	yA
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == '/') {   // 	v/	y/
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.25f) {
    return lnp->standard_kern-0.10f;
    }  
  }
else if (lc->previous_character == 'f') {	//					f
  if (lc->next_character == 'A') {   // 	fA
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == 'd') {   // 	fd
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == 't') {   // 	ft
    return lnp->standard_kern-0.1f;
    }
  else if (lc->next_character == 'j') {   // 	fj
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.5f) {
    return lnp->standard_kern-0.25f;    
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.25f) {
    return lnp->standard_kern-0.25f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.5f) { 
    return lnp->standard_kern-0.25f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.6f) {
    return lnp->standard_kern-0.25f;
    }  
  }
else if (lc->previous_character_boundary.upper_right.y <= 0.25f) {	//			 tiny before
  if (lc->next_character == 'V') {   // 		,V
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == '\\') { // 		,\\ backslash
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character == 't') { // 	        ,t
    return lnp->standard_kern-0.1f;
    }
  else if (lc->next_character == 'T') { // 	        .T
    return lnp->standard_kern-0.3f;
    }
  }
else if (lc->previous_character_boundary.upper_right.y <= 0.5f) { //			mid before
  if (lc->next_character == 'V') {   // 		cV
    return lnp->standard_kern-0.15f;
    }
  else if (lc->next_character == '\\') { // 		c\\ backslash
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == 't') { // 		ct
    return lnp->standard_kern-0.1f;
    }
  else if (lc->next_character == 'T') { // 		cT
    return lnp->standard_kern-0.3f;
    }
  }
else if (lc->previous_character_boundary.upper_right.y <= 0.6f) { //			big before
  if (lc->next_character == 'V') {   //			
    return lnp->standard_kern-0.10f;
    }
  else if (lc->next_character == '\\') { // 	b\	h\	k\ backslash
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == 'T') { // 	bt	ht	kt
    return lnp->standard_kern-0.3f;
    }
  }
else if (lc->previous_character == 'F') {	//					F
  if (lc->next_character == 'A') {   // 	FA
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == '/') {   // 	F/
    return lnp->standard_kern-0.25f;
    }
  }
else if (lc->previous_character == '/') {	//					/
  if (lc->next_character == 'A') {   // 	/A
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == '/') {   // 	//
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == 'd') {   // 	/d
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.25f) {
    return lnp->standard_kern-0.25f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.5f) { 
    return lnp->standard_kern-0.3f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.6f) { 
    return lnp->standard_kern-0.25f;
    }      
  }
else if (lc->previous_character == 'T') {	//					T
  if (lc->next_character == 'A') {   // 	TA
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == '/') {   // 	T/
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.25f) {
    return lnp->standard_kern-0.30f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.5f) { 
    return lnp->standard_kern-0.3f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.6f) { 
    return lnp->standard_kern-0.30f;
    }      
  }
else if (lc->previous_character == 'T') {	//					T
  if (lc->next_character == 'A') {   // 	TA
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == '/') {   // 	T/
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.25f) {
    return lnp->standard_kern-0.30f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.5f) { 
    return lnp->standard_kern-0.3f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.6f) { 
    return lnp->standard_kern-0.30f;
    }      
  }
else if (lc->previous_character == 'Y') {	//					Y
  if (lc->next_character == 'A') {   // 	YA
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character == '/') {   // 	Y/
    return lnp->standard_kern-0.5f;
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.25f) {
    return lnp->standard_kern-0.30f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.5f) { 
    return lnp->standard_kern-0.3f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.6f) { 
    return lnp->standard_kern-0.23f;
    }      
  }
else if (lc->previous_character == 'V') {	//					V
  if (lc->next_character == 'A') {   // 	VA
    return lnp->standard_kern-0.25f;
    }
  else if (lc->next_character == '/') {   // 	V/
    return lnp->standard_kern-0.3f;
    }
  else if (lc->next_character_boundary.upper_right.y <= 0.25f) {
    return lnp->standard_kern-0.17f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.5f) { 
    return lnp->standard_kern-0.15f;
    }  
  else if (lc->next_character_boundary.upper_right.y <= 0.6f) { 
    return lnp->standard_kern-0.10f;
    }  
  }
return (lnp->standard_kern);
}


// end of file

