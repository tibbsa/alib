// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
// AL_BUG.H: ALIB; BugFind macro                                          //
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
#ifndef __AL_BUG_H
#define __AL_BUG_H

#ifdef DEBUG
   #define Set_Alert_File(file) static char __ALERTFilename[] = file;

   #define Alert(expr) if (expr) __ALERT (__ALERTFilename, #expr, __LINE__);
#else
   #define Set_Alert_File(file)
   #define Alert(expr)
#endif
void __ALERT (char *filename, char *condition, word l);

#endif
