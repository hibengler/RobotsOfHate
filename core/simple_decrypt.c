#include <stdio.h>
#include "simple_code_data1.h"
/*
Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2018 Hibbard M. Engler

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











unsigned char *decrypt1(unsigned char *code,int length,int key) {
unsigned int i;
int j;
i = (key*982451501) & CODE_DATA1_AND;
for (j=0;j<length;j++) {
  code[j] = code[j] ^ code_data1[i];
  i= (i+1)  & CODE_DATA1_AND;
  }
return code;
}




