// ���������������������������������������������������������������������� //
// AL_MTSK.H: ALIB; Multitasker timeslicing declarations                  //
// ���������������������������������������������������������������������� //
#ifndef __AL_MTSK_H
#define __AL_MTSK_H

// ����������������������������������������������������������������������� 
// TASKER TYPES          
// �����������������������������������������������������������������������
#define MTASK_DOS          1
#define MTASK_OS2          2
#define MTASK_WINDOWS      3
#define MTASK_DV           4

// ����������������������������������������������������������������������� 
// CLASS DECLARATION     
// �����������������������������������������������������������������������
class Operating_System
{
private:
   char  VerHi;                  // Version, hi byte
   char  VerLo;                  // Version, low byte
   char  Type;                   // Multitasker type (see MTASK_xxx)

   char  Name [35];              // All of the above

public:
   Operating_System();
   char *GetVerString (void) {return Name;}
   void Slice (void);
};

#endif
