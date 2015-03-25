/*
    NINJAM ASIO driver - njasiodrv_if.cpp
    Copyright (C) 2005 Cockos Incorporated

    NJASIODRV is dual-licensed. You may modify and/or distribute NJASIODRV under either of
    the following licenses:

      This software is provided 'as-is', without any express or implied
      warranty.  In no event will the authors be held liable for any damages
      arising from the use of this software.

      Permission is granted to anyone to use this software for any purpose,
      including commercial applications, and to alter it and redistribute it
      freely, subject to the following restrictions:

      1. The origin of this software must not be misrepresented; you must not
         claim that you wrote the original software. If you use this software
         in a product, an acknowledgment in the product documentation would be
         appreciated but is not required.
      2. Altered source versions must be plainly marked as such, and must not be
         misrepresented as being the original software.
      3. This notice may not be removed or altered from any source distribution.


    or:

      NJASIODRV is free software; you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation; either version 2 of the License, or
      (at your option) any later version.

      NJASIODRV is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with NJASIODRV; if not, write to the Free Software
      Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  This file provides the basic code for loading NJASIODRV.DLL from the NINJAM
  application.
*/


#ifndef NO_SUPPORT_ASIO

#include "audiostream.h"
#include <stdio.h>


static const char* ASIO_DLL_FILENAME     = "njasiodrv.dll" ;
static const char* ASIO_CONSTRUCTOR_NAME = "create_asio_streamer" ;
static const char* ASIO_ARGS_FMT         = "%d:%d,%d:%d,%d" ;

static audioStreamer* (*newASIO)(char** cli_args , SPLPROC on_samples_proc) ;


static bool load_njasiodrv_dll()
{
  static HINSTANCE njasiodrv ; if (!!njasiodrv && !!newASIO) return true ;

  char buf[1024] ;
  GetModuleFileName(GetModuleHandle(NULL) , buf , sizeof(buf)) ;
printf("init_dll() GetModuleFileName()=%s" , buf) ;
  char *p = buf ; while (*p) p++ ; while (p >= buf && *p != '\\') p-- ;
  strcpy(++p , ASIO_DLL_FILENAME) ;
printf("init_dll() LoadLibrary()=%s" , buf) ;

  if (njasiodrv = LoadLibrary(buf))
    *((void**)&newASIO) = (void *)GetProcAddress(njasiodrv , ASIO_CONSTRUCTOR_NAME) ;

  return !!njasiodrv && !!newASIO ;
}

bool is_njasiodrv_avail() { return load_njasiodrv_dll() ; }

audioStreamer *create_audioStreamer_ASIO_DLL(SPLPROC on_samples_proc , int asio_driver_n ,
                                             int     in_channel1_n   , int in_channel2_n ,
                                             int     out_channel1_n  , int out_channel2_n)
{
// from njasiodrv_if.cpp
//  ints like: [ driverN [ + ":" + input_channel1_n  [ + "," + input2channelN  ] ]
//                       [ + ":" + output1channelN [ + "," + output2channelN ] ] ]
  char buf[2050] ; char* cli_args = buf ; // 1025 wchars wsprintf max
  wsprintf(buf , ASIO_ARGS_FMT , asio_driver_n                  ,
                                 in_channel1_n  , in_channel2_n ,
                                 out_channel1_n , out_channel2_n) ;

  return (load_njasiodrv_dll()) ? newASIO(&cli_args , on_samples_proc) : NULL ;
}

#endif // NO_SUPPORT_ASIO
