/*
    NINJAM - audiostream.h
    Copyright (C) 2005 Cockos Incorporated

    NINJAM is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    NINJAM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NINJAM; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  This header is used by NINJAM clients to define an abstract audio streamer interface,
  as well as declare functions for creating instances of these audio streamers.

  On the legacy Win32 client, these functions are primarily called from audioconfig.cpp,
  and on the legacy Cocoa client the function is called from Controller.mm.
  Clients targeting other platforms would call these directly.

  The basic structure is:
    The client runs, creates an audioStreamer (below), giving it a SPLPROC,
    which is a client owned function that then in turn calls NJClient::AudioProc.

  But this is just the interface declaration etc.

*/

#ifndef _AUDIOSTREAM_H_
#define _AUDIOSTREAM_H_


/*\ windows audio API support
|*| in order to disable support for individual audio APIs
|*|    the following switches may be defined prior to inclusion
|*| e.g.
|*|   #define NO_SUPPORT_ASIO
|*|   #define NO_SUPPORT_KS
|*|   #define NO_SUPPORT_DS
|*|   #define NO_SUPPORT_WAVE
|*|   #include <ninjam/audiostream.h>
\*/


#ifdef _WIN32
#  ifndef NO_SUPPORT_DS
#    include <dsound.h>
#  endif // NO_SUPPORT_DS
#endif // _WIN32


class NJClient ;


class audioStreamer
{
public:

  audioStreamer()
  {
    m_innch  = DEFAULT_N_INPUTS ;
    m_outnch = DEFAULT_N_OUTPUTS ;
    m_bps    = DEFAULT_BITDEPTH ;
    m_srate  = DEFAULT_SAMPLERATE ;
  }
  virtual ~audioStreamer() { }

  virtual const char* GetChannelName(int idx)       = 0 ;
  virtual const char* GetInputChannelName(int idx)  { return GetChannelName(idx) ; }
  virtual const char* GetOutputChannelName(int idx) { return GetChannelName(idx) ; }
  virtual       bool  addInputChannel()             { return false ; }
  virtual       bool  addOutputChannel()            { return false ; }


  int m_srate ;
  int m_innch ;
  int m_outnch ;
  int m_bps ;


  enum WinApi { WIN_AUDIO_ASIO , WIN_AUDIO_KS   ,
                WIN_AUDIO_DS   , WIN_AUDIO_WAVE } ;
  enum NixApi { NIX_AUDIO_JACK , NIX_AUDIO_ALSA } ;

  static const int DEFAULT_N_INPUTS   = 2 ;
  static const int DEFAULT_N_OUTPUTS  = 2 ;
  static const int DEFAULT_BITDEPTH   = 16 ;
  static const int DEFAULT_SAMPLERATE = 48000 ;
} ;


typedef void (*SPLPROC)(float** input_buffer  , int n_input_channels  ,
                        float** output_buffer , int n_output_channels ,
                        int     n_samples     , int sample_rate       ) ;


#ifdef _WIN32
#  ifndef NO_SUPPORT_ASIO
bool           is_njasiodrv_avail() ;
audioStreamer* create_audioStreamer_ASIO_DLL(SPLPROC on_samples_proc , int asio_driver_n ,
                                             int     in_channel1_n   , int in_channel2_n ,
                                             int     out_channel1_n  , int out_channel2_n) ;
#  endif // NO_SUPPORT_ASIO

#  ifndef NO_SUPPORT_KS
audioStreamer* create_audioStreamer_KS(int  sample_rate     , int     bit_depth      ,
                                                              int*    n_buffers      ,
                                       int* buffer_size     , SPLPROC on_samples_proc) ;
#  endif // NO_SUPPORT_KS

#  ifndef NO_SUPPORT_DS
audioStreamer* create_audioStreamer_DS(int  sample_rate     , int     bit_depth      ,
                                       GUID device_guids[2] , int*    n_buffers      ,
                                       int* buffer_size     , SPLPROC on_samples_proc) ;
#  endif // NO_SUPPORT_DS

#  ifndef NO_SUPPORT_WAVE
audioStreamer* create_audioStreamer_WO(int  sample_rate     , int     bit_depth      ,
                                       int  device_ns[2]    , int*    n_buffers      ,
                                       int* buffer_size     , SPLPROC on_samples_proc) ;
#  endif // NO_SUPPORT_WAVE

#else // _WIN32

#  ifdef _MAC
audioStreamer* create_audioStreamer_CoreAudio(char**  audio_devices   , int sample_rate ,
                                              int     n_channels      , int bit_depth   ,
                                              SPLPROC on_samples_proc                   ) ;
#  else // _MAC

audioStreamer* create_audioStreamer_JACK(const char* jack_client_name  ,
                                         int         n_input_channels  ,
                                         int         n_output_channels ,
                                         SPLPROC     on_samples_proc   ,
                                         NJClient*   a_NJClient        ) ;
audioStreamer* create_audioStreamer_ALSA(char* cli_args , SPLPROC on_samples_proc) ;
audioStreamer* create_audioStreamer_ALSA(SPLPROC     on_samples_proc          ,
                                         const char* input_device  = "hw:0,0" ,
                                         const char* output_device = "hw:0,0" ,
                                         int         n_channels    = 2        ,
                                         int         sample_rate   = 44100    ,
                                         int         bit_depth     = 16       ,
                                         int         n_buffers     = 16       ,
                                         int         buffer_size   = 1024     ) ;

#  endif // _MAC
#endif // _WIN32

#endif // _AUDIOSTREAM_H_
