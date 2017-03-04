typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ //
// ALIB.H: ALIB; General declarations used in ALIB                        //
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ //
#ifndef __ALIB_H
#define __ALIB_H

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// COMPILER INCLUDE FILES
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include <stdlib.h>
#include <string.h>
#include <share.h>
#include <ctype.h>
#include <io.h>
#include <errno.h>
#include <fcntl.h>
#include <mem.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <dir.h>
#include <msgapi.h>
#include <sys/stat.h>
#include <dos.h>

#include <al_kbd.h>
#include <al_mtsk.h>
#include <al_log.h>
#include <al_bug.h>
#include <al_mem.h>
#include <al_tmr.h>
#include <al_mapi.h>
#include <al_tg.h>
#include <al_str.h>
#include <al_file.h>
#include <al_crc32.h>
#include <al_mbox.h>
#include <al_td.h>
#include <al_fos.h>
#include <al_jam.h>

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// GLOBAL DECLARATIONS/DEFINITIONS
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define True         1
#define False        0
#define LoByte(x) ((unsigned char)((x) & 0xff))
#define HiByte(x) ((unsigned char)((unsigned short)(x) >> 8))

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// THE 'ALibrary' CLASS
//    The ALibrary class is used as the "glue" of my library.  All of the
//    memory allocation/tracking structures are kept in here.
//
//    The library will be initialized as soon as this object has been
//    created.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
class ALibrary
{
private:
   int         share_detected;
   int         Check_Sharing (void);
public:
   ALibrary();
  ~ALibrary();
   int         ShareOK (void) {return (share_detected);}
   LogFile     log;
};

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// GLOBAL VARIABLES (FOR THE ENTIRE LIBRARY)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
extern ALibrary alib;
extern Operating_System os;
extern Fossil Modem;

#endif   /* __ALIB_H */
