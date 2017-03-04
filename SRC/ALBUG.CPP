// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 
// ALBUG.CPP: ALIB; ALERT macro implementation
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// Implements the ALERT macro (like Assert, but asserts ONLY if the expr
// is TRUE)
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 

#include <alib.h>
Set_Alert_File ("ALBUG.CPP<library ALIB>")
void __ALERT (char *filename, word l);

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void __ALERT (char *filename, word l);
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *filename     Source file the ALERT occured in
//            word l             Line number of ALERT
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Alerts the user (via a line at the bottom of the screen) that a BUG may
// have been found!
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void __ALERT (char *filename, char *condition, word l)
{
int st=1;
char buf [2048];
int old=directvideo;
Timer tmr;
int IMMEDIATE_QUIT=0;

   while (xkbhit())
      xkbflush();

   if (old)
      directvideo=0;
   
   gettext (1, 22, 80, 25, buf);
   textattr (4|(7<<4));

   alib.log.Put ("!ALERT at line %u of %s!", l, filename);
   alib.log.Put ("!ALERT condition: %s", condition);

   gotoxy (1, 22);
   clreol();
   gotoxy (1, 23);
   clreol();
   gotoxy (1, 24);
   clreol();
   gotoxy (1, 25);
   clreol();

   gotoxy (1, 22);
   textcolor ((4|(7<<4)) | BLINK);
   cprintf ("BUG ALERT: ");
   textattr (0|(7<<4));
   cprintf ("Internal error detected on line %u of %s!\r\n", l, filename);
   gotoxy (1, 23);
   textcolor (4|(7<<4));
   cprintf ("Condition: ");
   textcolor (0|(7<<4));
   cprintf ("%s", condition);

   gotoxy (1, 24);
   textattr (1|(7<<4));
   cprintf ("Program will abort automatically in 30 seconds!");

   gotoxy (1, 25);
   textattr (0|(7<<4));
   cprintf ("Press CTRL-A to continue running application, or any other key to abort");

   directvideo = old;
   tmr.set (30);

   while (!xkbhit())
   {
      if (!tmr)
      {
         nosound();
            if (xkbhit())
               break;
         IMMEDIATE_QUIT=1;

         break;
      }

      for (st =0; st < 10 && !xkbhit(); st++)
      {
         if (xkbhit())
            break;

         nosound();
         delay (35);
         sound (1000);
         delay (35);
         sound (800);
         delay (45);
         sound (1000);
         delay (35);
         sound (800);
         delay (55);
         sound (1000);
         delay (35);
         sound (800);
         delay (45);
         sound (1000);
         delay (35);
         sound (800);
         delay (35);
         sound (1000);
         delay (35);
         sound (800);
         if (xkbhit())
            break;
         delay (75);

         nosound();

         for (int xya=0; xya < 100; xya++)
         {
            if (xkbhit())
               break;
            delay (10);
         }
      }
   }

   switch (IMMEDIATE_QUIT ? 0x04 : xgetch())
   {
      case 0x01:  puttext (1, 22, 80, 25, buf);
                  nosound();
                  alib.log.Put ("!ALERT continuing");

                  return;
      default  :  puttext (1, 22, 80, 25, buf);
                  nosound();
                  if (IMMEDIATE_QUIT)
                     printf ("\n\nAutomatic 30 second abort!");
                  printf ("\n\nPlease send your log file to the author.\n");
                  alib.log.Put ("!ALERT aborting application");
                  alib.log.Close();
                  abort();
   }
}
   

