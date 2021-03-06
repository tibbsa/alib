// ���������������������������������������������������������������������� 
// ALKBD.CPP: ALIB; Keyboard implementations
// ���������������������������������������������������������������������� 
// Keyboard related functions (getch() for extended keys, etc.)
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALKBD.CPP<library ALIB>")

// ����������������������������������������������������������������������� 
// int xgetch (void)       
// �����������������������������������������������������������������������
// Arguments:
// Return:    Scancode from keyboard (see Key_xxx in ALKBD.H)
// �����������������������������������������������������������������������
// Retrieves a key from the keyboard, handling extended keys/combinations
// accordingly.
// �����������������������������������������������������������������������
int xgetch (void)
{
int key;
   asm {
      mov   ah, 10h
      int   16h
      mov   key, ax
   }
   
   switch (LoByte (key))
   {
      case 0:
         key = HiByte (key) + 256;
         break;

      case 0xE0:
         key = HiByte (key) + 512;
         break;

      default:
         if (HiByte(key) == 0xE0)
            key = LoByte(key) + 512;
         else
         {
            if (ispunct(LoByte(key)) && HiByte(key) > 0x36)
               key = LoByte(key) + 512;
            else
               key = LoByte(key);
         }
   }

   return key;
}


#define Head (*((unsigned short far *)MK_FP(0x40, 0x1A)))
#define Tail (*((unsigned short far *)MK_FP(0x40, 0x1C)))

// ����������������������������������������������������������������������� 
// int xkbhit (void)       
// �����������������������������������������������������������������������
// Arguments:
// Return:    True if a key is waiting, False otherwise
// �����������������������������������������������������������������������
// Checks the keyboard buffer to determine whether or not a key is waiting
// to be received.
// �����������������������������������������������������������������������
int xkbhit (void)
{
int ret;

   disable();
   ret = Head - Tail;
   enable();

   return ret;
}

// ����������������������������������������������������������������������� 
// void xkbflush (void)    
// �����������������������������������������������������������������������
// Arguments:
// Return:    
// �����������������������������������������������������������������������
// Flushes any pending keypress from the keyboard buffer
// �����������������������������������������������������������������������
void xkbflush (void)
{
   disable();
   Head = Tail;
   enable();
}

