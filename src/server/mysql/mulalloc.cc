/* Copyright (c) 2000, 2002, 2003, 2007 MySQL AB
   Use is subject to license terms

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA */

// #include "mysys_priv.h"
#include "include/my_sys.h"
#include "include/my_global.h"
#include <stdarg.h>
#include <cstring>

/*
  Malloc many pointers at the same time
  Only ptr1 can be free'd, and doing this will free all
  the memory allocated. ptr2, etc all point inside big allocated
  memory area.

  SYNOPSIS
    my_multi_malloc()
      myFlags              Flags
	ptr1, length1      Multiple arguments terminated by null ptr
	ptr2, length2      ...
        ...
	NULL
*/

void* my_multi_malloc(myf myFlags, ...)
{
  va_list args;
  char **ptr,*start,*res;
  size_t tot_length,length;
  // DBUG_ENTER("my_multi_malloc");

  va_start(args,myFlags);
  tot_length=0;
  while ((ptr=va_arg(args, char **)))
  {
    length=va_arg(args,uint);
    tot_length+=ALIGN_SIZE(length);
  }
  va_end(args);

  if (!(start=(char *) malloc(tot_length)))
    DBUG_RETURN(0); /* purecov: inspected */

  if (myFlags & MY_ZEROFILL)
      memset(start, 0, tot_length);

  va_start(args,myFlags);
  res=start;
  while ((ptr=va_arg(args, char **)))
  {
    *ptr=res;
    length=va_arg(args,uint);
    res+=ALIGN_SIZE(length);
  }
  va_end(args);
  DBUG_RETURN((void*) start);
}
