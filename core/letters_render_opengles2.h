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
/*$Revision: 1.5 $*/
#ifndef LETTERS_OPENGL_RENDER_H
#define LETTERS_OPENGL_RENDER_H
#include "glue.h"
#include "helper.h"
#include "letters.h"

extern void letters_opengles_setgraphics(GLuint xgProgram,int xmColorHandle,GLuint xgvPositionHandle,GLuint xmPositionHandle);

extern void lg_wide_line(lp p1,lp p2,fl line_width);

extern unsigned int letters_opengles_color_handle();
extern unsigned int letters_opengles_program_handle();



#endif
