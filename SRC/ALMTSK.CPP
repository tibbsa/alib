// ���������������������������������������������������������������������� 
// ALMTSK.CPP: ALIB; Multitasker identification/timeslice routines 
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALMTSK.CPP<library ALIB>")

// ����������������������������������������������������������������������� 
// GLOBAL VARIABLES (FOR THE ENTIRE LIBRARY)
// �����������������������������������������������������������������������
Operating_System os;

// �����������������������������������������������������������������������
// Operating_System::Operating_System()
// �����������������������������������������������������������������������
// Arguments:
// Return:
// �����������������������������������������������������������������������
// Searches for a multitasker/OS and fills in all the data.
// �����������������������������������������������������������������������
Operating_System::Operating_System()
{
char t[40];
union REGS regs;

   if (_osmajor < 10)
   {
      Type = MTASK_DOS;
      sprintf (t, "DOS %d.%d", _osmajor, _osminor);
      VerHi = _osmajor;
      VerLo = _osminor;
   }
   else
   {
      Type = MTASK_OS2;
      sprintf (t, "OS/2 DOS Box %d.%d", _osmajor/10, _osminor);
      VerHi = _osmajor/10;
      VerLo = _osminor;
   }

   regs.x.ax = 0x4680;
   int86 (0x2f, &regs, &regs);
   if (!regs.x.ax)
   {
      Type = MTASK_WINDOWS;
      strcpy (t, "Windows 3.0 Standard");
   }
   else
   {
      regs.x.ax = 0x1600;
      int86 (0x2f, &regs, &regs);
      Type = MTASK_WINDOWS;
      switch (regs.h.al)
      {
         case 0x00:
         case 0x80:
         case 0x81:
         case 0xFF: break;
         default  : sprintf (t, "Windows %d.%d Enhanced", regs.h.al, regs.h.ah);
                    VerHi = regs.h.ah;
                    VerLo = regs.h.al;
      }
      if (regs.h.al == 4)
         strcpy (t, "Windows 95");

      if (!strcmpi(getenv("OS"), "Windows_NT"))
         sprintf (t, "Windows NT %d.%d", VerHi, VerLo);
   }

   regs.x.cx = 0x4445;
   regs.x.dx = 0x5351;
   regs.x.ax = 0x2B01;
   int86 (0x21, &regs, &regs);
   if (regs.h.al != 0xFF)
   {
      Type = MTASK_DV;
      VerHi = regs.h.bh;
      VerLo = regs.h.bl;
      sprintf (t, "DESQview %d.%d", VerHi, VerLo);
   }

   strcpy (Name, t);
}

// �����������������������������������������������������������������������
// void Operating_System::Slice (void)
// �����������������������������������������������������������������������
// Arguments:
// Return:
// �����������������������������������������������������������������������
// Releases a timeslice (if appropriate to do so)
// �����������������������������������������������������������������������
void Operating_System::Slice (void)
{
   switch (Type)
   {
      case MTASK_DOS: return;
      case MTASK_WINDOWS:
      case MTASK_OS2:      asm {
                              mov ax, 1680h
                              int 2Fh
                           }
                           return;
      case MTASK_DV: asm {
                        mov   ax, 1000h
                        int   15h
                     }
                     return;
      default: return;
   }
}
