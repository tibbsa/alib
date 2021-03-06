// ���������������������������������������������������������������������� 
// ALIB.CPP: ALIB; Library Global Data Definition File                    
//                 ALibrary implementation
// ���������������������������������������������������������������������� 
// This file is used to declare global variables (log files and such)     
// used in ALIB.
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALIB.CPP<library ALIB>")

// ����������������������������������������������������������������������� 
// GLOBAL VARIABLES (FOR THE ENTIRE LIBRARY)
// �����������������������������������������������������������������������
ALibrary alib;

// ����������������������������������������������������������������������� 
// ALibrary::ALibrary()
// �����������������������������������������������������������������������
// Arguments:
// Return:
// �����������������������������������������������������������������������
// Initializes the library (checks for SHARE.EXE). 
// �����������������������������������������������������������������������
ALibrary::ALibrary()
{
   share_detected = Check_Sharing();
#ifdef DEBUG
   unlink ("memtrk.dbg");
#endif
}

// ����������������������������������������������������������������������� 
// ALibrary::~ALibrary()
// �����������������������������������������������������������������������
// Arguments:
// Return:
// �����������������������������������������������������������������������
// Deinitializes the library (frees any allocated memory)
// �����������������������������������������������������������������������
ALibrary::~ALibrary()
{
#ifdef DEBUG
   if (MemTrkDiagnostics())
   {
      MemTrkDump();
      MemTrkCleanup();
   }

   unlink ("memtrk.dbg");
#endif
   log.Close();
}

// ����������������������������������������������������������������������� 
// int ALibrary::Check_Sharing (void)
// �����������������������������������������������������������������������
// Arguments:
// Return:    True if sharing is OK, False otherwise.
// �����������������������������������������������������������������������
// Checks to see if SHARE.EXE is loaded.  Returns True or False accordingly.
// �����������������������������������������������������������������������
int ALibrary::Check_Sharing (void)
{
   asm {
      mov   ax, 1000h
      int   2Fh
      cmp   al, 0FFh
      je    Share_OK

      jmp   Share_Not_OK
   }
Share_OK:
   share_detected = 1;
   return 1;

Share_Not_OK:
   share_detected = 0;
   return 0;
}

