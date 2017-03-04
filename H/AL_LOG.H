// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ //
// AL_LOG.H: ALIB; Logging routines                                       //
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ //
#ifndef __AL_LOG_H
#define __AL_LOG_H

#define LOG_FULL_MASK   "ABCDEFGHIJKLMNOPQRSTUVWXYZ`~1!2@3#4$5%6^7&8*9(0)-_=+\\|/:><"

// ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ 
// CLASS DECLARATION     
// ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
class LogFile
{
private:
   char  Filename [120];         // Log file name
   char  Mask [70];              // Log mask
   char  Program [5];            // Program name (4 bytes)

   int   ForceFlush;             // ALWAYS flush the log file?
   FILE *fp;

public:
   LogFile() {*Filename = '\0'; fp=NULL;}
  ~LogFile() {Close();}
   int   Open (char *fname, char *mask, char *pgm, int flush);
   void  Close (void);
   void  Put (char *fmt, ...);
};

#endif
