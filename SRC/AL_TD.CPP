// ���������������������������������������������������������������������� 
// AL_TD.CPP: ALIB; Time/date functions           
// ���������������������������������������������������������������������� 
// Implements time/date functions
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALTD.CPP<library ALIB>")

// �����������������������������������������������������������������������
// int GetDow (void)
// �����������������������������������������������������������������������
// Arguments:  
// Return:     Integer from 0-6
// �����������������������������������������������������������������������
// Returns the day of the week (an integer from 0=Sunday to 6=Saturday)
// �����������������������������������������������������������������������
int GetDow (void)
{
dosdate_t today;

   _dos_getdate (&today);
   return (today.dayofweek);
}

