// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 
// ALMSGBOX.CPP: ALIB; Messagebox implementation
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// Implements a MsgBox function for displaying momentary messages.
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 

#include <alib.h>
Set_Alert_File ("ALMSGBOX.CPP<library ALIB>")

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void MsgBox (int timeout, int force, char border, char text, char *msg);
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments:  int timeout          Max # seconds to display msg.
//             int force            FORCE the message to stay there for timeout
//             char border,text     Attributes
//             char *msg            Text to display
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Displays a momentry message to the user.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void MsgBox (int timeout, int force, char border, char text, char *msg)
{
   Alert (timeout == 0);
   Alert (border == 0);
   Alert (text == 0);
   Alert (msg == NULL);
   Alert (*msg == NUL);

TWindow w;
Timer t;
int l;
int c,m;
int x1, y1, x2, y2;
   c=11;
   m=40;

   l=strlen(msg);
   x1=m-(l/2)-2;
   y1=c;
   x2=m+(l/2)+2;
   y2=c+4;

   if (!w.Open (x1, y1, x2, y2, BDR_SINGLE, border, text))
      return;

   w.Write (2, 2, msg);
   t.set (timeout);
   while (t)
   {
      if (!force)
      {
         if (xkbhit())
         {
            while (xkbhit())
               xgetch();
            w.Close();
            return;
         }
      }

      os.Slice();
   }

   w.Close();
   while (xkbhit())
      xgetch();
}


