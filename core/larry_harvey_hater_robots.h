/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2019 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/






/*$Revision: 1.2 $*/

#ifndef DASOUND24_LARRY_HARVEY_ROBOT_H
#define DASOUND24_LARRY_HARVEY_ROBOT_H

#include "glue.h"
#include "helper.h"



extern int example_raylib_larry_harvey_setupGraphics(int w, int h);

extern void example_raylib_larry_harvey_renderBackground(void);
extern void example_raylib_larry_harvey_renderFrame(void);
extern void example_raylib_larry_harvey_renderForeground(void);
extern void example_raylib_larry_harvey_touchEvent(int action,float x,float y);
extern void example_raylib_larry_harvey_renderButton(int program,
   int mColorHandle,int menu_position_handle,glMatrix *view);


#endif

