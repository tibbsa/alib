// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ //
// AL_MAPI.H: ALIB; MsgAPI Wrapper                                        //
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ //
#ifndef __AL_MAPI_H
#define __AL_MAPI_H

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// CLASS DECLARATION - MsgAPI Object
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
class MAPI
{
private:
   struct _minf      mi;
   int               Init_Complete;

public:
   int               Init (word dz);
   void              Close (void);
   MAPI();
  ~MAPI();
};

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// CLASS DECLARATION - Message Area Object
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
class MAREA
{
private:

public:
   HAREA             handle;
   int               MsgsOpen;
   MAREA();
  ~MAREA();

   int               open (char *path, word mode, word type);
   int               close (void);
   dword             GetHighWater (void);
   int               SetHighWater (dword hw);
   dword             GetCurMsg (void);
   dword             GetNumMsgs (void);
   dword             GetHighMsg (void);
   int               lock (void);
   int               unlock (void);

   int               kill_message (dword num);
   void              setlimits (dword msgs, dword skip, dword days);

};

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// CLASS DECLARATION - Message Object
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
class Message
{
private:
   HAREA       AreaHandle;
   HMSG        handle;
   int        *MsgsOpen;
   MAREA      *pMAREA;

public:
   XMSG        header;

   Message (MAREA &msgArea);

   int Open (word Mode, dword Num);
   void Close (void);
   int Read (dword StartOfs, dword MaxSize, byte *Txt, dword MaxCtrlSize,
             byte *Ctrl);
   int Write (word Append, byte *Txt, byte *Ctrl);
   dword GetTextLen (void);
   dword GetCtrlLen (void);
   UMSGID MsgnToUID (dword MsgNum);
   dword UIDToMsgn (word Type, UMSGID msgid);
   byte *GetCtrlToken (byte *Buffer, byte *Tag);
   void FreeCtrlToken (byte *CtrlToken);
   void RemoveCtrlToken (byte *Buffer, byte *Tag);
   word GetNumKludges (byte *Buffer);
   dword SQ_Hash (byte *Buffer);
};

#endif
