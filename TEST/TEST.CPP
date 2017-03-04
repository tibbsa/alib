#define DEBUG
#define MEMDEBUG
#include <alib.h>
Set_Alert_File ("TEST.CPP")

extern ALibrary alib;
extern Operating_System os;

main()
{
   if (alib.ShareOK())
      cprintf ("* Share OK\r\n");
   
   cprintf ("OS: %s\r\n", os.GetVerString());
   alib.log.Open ("TEST.LOG", "", "TEST", True);
   alib.log.Put ("-");
   alib.log.Put ("+begin, Test");

   alib.log.Put ("+end, Test");
   alib.log.Close();
   return 0;
}

         
