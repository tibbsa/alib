// ���������������������������������������������������������������������� 
// ALTIMER.CPP: ALIB; 1-second granularity timer implementation
// ���������������������������������������������������������������������� 
// Implements a 1-second granularity timer
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALTIMER.CPP<library ALIB>")

// �����������������������������������������������������������������������
// void Timer::set (time_t length)
// �����������������������������������������������������������������������
// Arguments: time_t length      Length (seconds) to set timer to
// Return:    
// �����������������������������������������������������������������������
// Initializes a timer
// �����������������������������������������������������������������������
void Timer::set (time_t length)
{
   Alert (length == 0L);
   t_Set = time (NULL);
   t_Expires = t_Set + length;
}

// �����������������������������������������������������������������������
// int Timer::expired (void)
// �����������������������������������������������������������������������
// Arguments: 
// Return:    # of seconds until expiry
// �����������������������������������������������������������������������
// Returns the number of seconds until expiry, or 0 if the timer has
// expired.
// �����������������������������������������������������������������������
int Timer::expired (void)
{
   Alert (t_Set == 0L);
   Alert (t_Expires == 0L);
   Alert (t_Set == t_Expires);

   if (t_Expires >= time(NULL))
      return ((int)(t_Expires - time(NULL)));
   else
      return 0;
}
