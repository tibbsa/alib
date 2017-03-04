// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ //
// AL_TMR.H: ALIB; Low-accuracy timer declarations                        //
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ //
#ifndef __AL_TMR_H
#define __AL_TMR_H

// ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ 
// CLASS DECLARATION     
// ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
class Timer
{
private:
   time_t   t_Set;               // time_t when timer set
   time_t   t_Expires;           // time_t when timer is due to expire

   
public:
   Timer() {t_Set=0L; t_Expires=0L;}
   void set (time_t length);
   int  expired (void);
   operator int() {return expired();}
};

#endif
