// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 
// ALLOG.CPP: ALIB; Log file implementation
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// Maskable (i.e. certain lines only) log output/control.
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 

#include <alib.h>
Set_Alert_File ("ALLOG.CPP<library ALIB>")

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// int LogFile::Open (char *fname, char *mask, char *pgm, int flush);
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *fname     Filename for the log
//            char *mask      Mask to use on the log output
//            char *pgm       Program name (4 letter max)
//            int flush       Flush log file every time?
// Return:    True if OK, False if an error occurs
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Opens the log file
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int LogFile::Open (char *fname, char *mask, char *pgm, int flush)
{
   Alert (*fname == '\0');
   Alert (fp != NULL);

   if (!*mask)
      strcpy (Mask, LOG_FULL_MASK);
   else
      strcpy (Mask, mask);

   if (strlen (pgm) > 4)
      strcpy (Program, "    ");
   else
      strcpy (Program, pgm);

   ForceFlush = flush;

   strcpy (Filename, fname);

   fp = fopen (Filename, "a");
   if (!fp)
      return (False);

   return (True);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// void LogFile::Close (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Close the log file [if it is open]
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void LogFile::Close (void)
{
   if (fp)
   {
      fclose (fp);
      fp = NULL;
   }
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// void LogFile::Put (char *fmt, ...)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *fmt, ...  Parameters to be passed onto vsprintf()
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Writes an entry to the log file.  If fmt == '-', writes a blank line.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void LogFile::Put (char *fmt, ...)
{
char scratch [256];
char l [256];
char premsg [256];
char *Months[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                "Sep", "Oct", "Nov", "Dec"};
va_list va_Params;
struct dosdate_t dt_Date;
struct time tm_Time;

   if (!fp)
      return;

   Alert (*fmt == '\0');

   if (!strcmpi(fmt, "-"))
      fputs ("\n", fp);
   else
   {
   char *p=Mask;

      while (*p)
      {
         if (*p == *fmt)
            break;

         p++;
      }
      if (!*p)
         return;

      gettime (&tm_Time);
      _dos_getdate (&dt_Date);

      va_start (va_Params, fmt);
      vsprintf (scratch, fmt, va_Params);
      va_end (va_Params);

      sprintf (premsg, "%c %-2.2d %s %-2.2d:%-2.2d:%-2.2d %-4.4s  ", *fmt, dt_Date.day, Months [dt_Date.month - 1], tm_Time.ti_hour, tm_Time.ti_min, tm_Time.ti_sec, Program);
      strcpy (l, premsg);
      strcat (l, scratch+1);
      strcat (l, "\n");
      fputs (l, fp);
   }

   if (ForceFlush || *fmt == '!')
   {
      fflush (fp);
      Close();
      Open (Filename, Mask, Program, ForceFlush);
   }
}

