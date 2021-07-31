/*
Since this is derived from raylib, this has a dual licese zlib/libpng and GNU LGPL 2.0

*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2018 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
* 


Robots of Hate
Copyright (C)2021-2021 Hibbard M. Engler

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


This is a very abridged version of not_rol for larry_harvey_has_a_possee.

*/

#ifndef NOT_MINI_RL_H
#define NOT_MINI_RL_H 1

#ifdef __cplusplus
#define NOT_RL_CLITERAL
#else
#define NOT_RL_CLITERAL    (not_rl_Color)
#endif

                          
    // Vector2 type
    typedef struct not_rl_Vector2 {   
        float x;
        float y;
    } not_rl_Vector2;          
    
    // Vector3 type
    typedef struct not_rl_Vector3 {
        float x;
        float y;
        float z;
    } not_rl_Vector3;


    
   
    // Matrix type (OpenGL style 4x4 - right handed, column major)
    typedef struct not_rl_Matrix {
        float m0, m4, m8, m12;
        float m1, m5, m9, m13;
        float m2, m6, m10, m14;   
        float m3, m7, m11, m15;
    } not_rl_Matrix;


    // Color type, RGBA (32bit)
    typedef struct not_rl_Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } not_rl_Color;
    
    // Rectangle type
    typedef struct not_rl_Rectangle {
        int x;
        int y;
        int width;
        int height;
    } not_rl_Rectangle;
    



#endif
