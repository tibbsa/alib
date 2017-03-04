// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 
// AL_MAPI.CPP: ALIB; MsgAPI Implementation     
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// Implements the Squish MsgAPI library via a C++ wrapper.
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 

#include <alib.h>
Set_Alert_File ("ALMAPI.CPP<library ALIB>")

MAPI::MAPI()
{
#ifdef DEBUG
   alib.log.Put ("MInitializing MAPI object (%p)", this);
#endif

   Init_Complete = False;
}

MAPI::~MAPI()
{
   Close();
}

void MAPI::Close (void)
{
   if (Init_Complete)
   {
#ifdef DEBUG
      alib.log.Put ("MDeinitializing MAPI object (%p)", this);
#endif
      MsgCloseApi();
      Init_Complete = False;
   }
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int MAPI::Init (word dz)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: word dz            Default zone # for *.MSG areas  
// Return:    FALSE if an error occurs, TRUE otherwise
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Initializes the MsgAPI interface
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int MAPI::Init (word dz)
{
   memset (&mi, '\0', sizeof mi);
   mi.def_zone = dz;
   mi.req_version = MSGAPI_VERSION;

#ifdef DEBUG
   alib.log.Put ("MMAPI %p: Init - version %d, DZ=%d", this, mi.req_version, dz);
#endif
   if (MsgOpenApi (&mi) == -1)
   {
      alib.log.Put ("!Error initializing MsgAPI");
      return (False);
   }
   else
   {
      Init_Complete = True;
      return (True);
   }
}        

/* ----------------------------------------------------------------------- */
MAREA::MAREA()
{
#ifdef DEBUG
   alib.log.Put ("MInitializing MAREA object (%p)", this);
#endif
   MsgsOpen = 0;
   handle = NULL;
}

MAREA::~MAREA()
{
#ifdef DEBUG
   alib.log.Put ("MDeinitializing MAREA object (%p)", this);
#endif
   if (handle)
      close();
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int MAREA::open (char *path, word mode, word type)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *path         Path to message base/*.SQD files
//            word mode          Mode to open base in (MODE_CRIFNEC, etc.)
//            word type          Type of message base (*.MSG or *.SQ?)
// Return:    FALSE if an error occurs, TRUE otherwise
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Opens the message area in (path) of type (type)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int MAREA::open (char *path, word mode, word type)
{
   Alert (path == NULL);
   Alert (*path == '\0');
   Alert (mode != MSGAREA_NORMAL && mode != MSGAREA_CREATE && \
          mode != MSGAREA_CRIFNEC);
   Alert (type != MSGTYPE_SDM && type != (MSGTYPE_SDM|MSGTYPE_ECHO) && \
          type != MSGTYPE_SQUISH && type != (MSGTYPE_SQUISH|MSGTYPE_ECHO));
   Alert (handle != NULL);

#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Opening msgarea \"%s\" (mode=%d, type=%02Xh)", this, path, mode, type);
#endif
   handle = MsgOpenArea (path, mode, type);
   if (!handle)
   {
      alib.log.Put ("!Error opening message area \"%s\"", path);
      return (False);
   }

   return (True);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int MAREA::close (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    FALSE if an error occurs, TRUE otherwise
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Closes the message area (if it is open)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int MAREA::close (void)
{
int c;
   Alert (handle == NULL);

   if (MsgsOpen > 0)
   {
      Alert (MsgsOpen > 0);
#ifdef DEBUG
      alib.log.Put ("!MMAREA %p: Close area aborted - %d msgs open!", this, MsgsOpen);
#endif
      return (False);
   }

#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Closing area", this);
#endif

   c = MsgCloseArea (handle);
   Alert (c == -1);

   if (c == -1)
   {
      alib.log.Put ("!Error closing message area");
      return (False);
   }
   handle = NULL;
   return (True);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// dword MAREA::GetHighWater (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    High water mark in area
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Returns the high water mark for the message area.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
dword MAREA::GetHighWater (void)
{
   Alert (handle == NULL);
#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Returning HWM (%lu)", this, MsgGetHighWater(handle));
#endif
   return (MsgGetHighWater (handle));
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int MAREA::SetHighWater (dword hw);
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: dword hw           High water mark message #
// Return:    False if an error occurs
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Sets the high water mark for the message area.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int MAREA::SetHighWater (dword hw)
{
   Alert (handle == NULL);
#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Setting HWM (%lu)", this, hw);
#endif
   if (MsgSetHighWater(handle, hw) == -1)
   {
      alib.log.Put ("!Error setting high water mark");
      return (False);
   }
   else
      return (True);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// dword MAREA::GetCurMsg (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    Current message # in area
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Returns the current message number for the message area.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
dword MAREA::GetCurMsg (void)
{
   Alert (handle == NULL);
#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Returning msg# (%lu)", this, MsgGetCurMsg(handle));
#endif
   return (MsgGetCurMsg (handle));
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// dword MAREA::GetNumMsgs (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    Number of messages in area
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Returns the number of messages in the area.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
dword MAREA::GetNumMsgs (void)
{
   Alert (handle == NULL);
#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Getting #/msgs (%lu)", this, MsgGetHighMsg(handle));
#endif
   return (MsgGetHighMsg(handle));
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// dword MAREA::GetHighMsg (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    High message # in area
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Returns the high message number in the area.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
dword MAREA::GetHighMsg (void)
{
   Alert (handle == NULL);
#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Getting high message # (%lu)", this, MsgGetHighMsg(handle));
#endif
   return (MsgGetHighMsg(handle));
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int MAREA::lock (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    False if an error occured
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Locks the message area for exclusive use (high performance apps only)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int MAREA::lock (void)
{
   Alert (handle == NULL);
#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Locking base", this);
#endif
   if (MsgLock (handle) == -1)
   {
      alib.log.Put ("?Unable to lock message base");
      return (False);
   }
   else
      return (True);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int MAREA::unlock (void)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: 
// Return:    False if an error occured
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Unlocks the message area
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int MAREA::unlock (void)
{
   Alert (handle == NULL);
#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Unlocking base", this);
#endif
   if (MsgUnlock (handle) == -1)
   {
      alib.log.Put ("?Unable to unlock message base");
      return (False);
   }
   else
      return (True);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int MAREA::kill_message (dword num)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: dword num          Message number to delete
// Return:    False if an error occured
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Deletes a message from the area
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int MAREA::kill_message (dword num)
{
   Alert (handle == NULL);
   Alert (num == 0L);
   Alert (num > GetHighMsg());

#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Killing msg#%lu", this, num);
#endif
   if (MsgKillMsg (handle,  num) == -1)
   {
      alib.log.Put ("!Unable to delete message #%lu from base", num);
      return (False);
   }
   else
      return (True);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void MAREA::setlimits (dword msgs, dword skip, dword days)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: dword msgs         Max #/msgs to hold in the area
//            dword skip         #/msgs to skip at beginning of area
//            dword days         Max age of msgs to hold in area
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Sets the limits for a message area
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void MAREA::setlimits (dword msgs, dword skip, dword days)
{
   Alert (handle == NULL);

#ifdef DEBUG
   alib.log.Put ("MMAREA %p: Setting limits: msgs=%lu, skip=%lu, days=%lu", this, msgs, skip, days);
#endif
   SquishSetMaxMsg (handle, msgs, skip, days);
}

Message::Message (MAREA &msgArea)
{
#ifdef DEBUG
   alib.log.Put ("MMSG %p: Initializing Message object", this);
#endif
   AreaHandle = msgArea.handle;
   handle = NULL;
   pMAREA = &msgArea;

#ifdef DEBUG
   alib.log.Put ("MMSG %p: Parent AREA=%p, w/%d msgs open", this, (void *)&msgArea, pMAREA->MsgsOpen);
#endif
   memset (&header, '\0', sizeof header);
}

int Message::Open (word Mode, dword Num)
{
   if (!AreaHandle)
      return 0;

#ifdef DEBUG
   alib.log.Put ("MMSG %p: Opening #%ld in mode %d", this, Num, Mode);
#endif

   handle = MsgOpenMsg (AreaHandle, Mode, Num);
   if (handle == NULL)
   {
      alib.log.Put ("!Error #%d opening message #%ld", msgapierr, Num);
	   return 0;
   }

   pMAREA->MsgsOpen++;
#ifdef DEBUG
   alib.log.Put ("MMSG %p: Assigned msg#%d", this, pMAREA->MsgsOpen);
#endif
   return 1;
}

void Message::Close (void)
{
   Alert (handle == NULL);
   
   MsgCloseMsg (handle);
#ifdef DEBUG
   alib.log.Put ("MMSG %p: Closing msg #%d", this, pMAREA->MsgsOpen);
#endif
   pMAREA->MsgsOpen--;
   handle = NULL;
}

int Message::Read (dword StartOfs, dword MaxSize, byte *Txt, dword MaxCtrlSize,
                   byte *Ctrl)
{
   if (!handle)
      return 0;

#ifdef DEBUG
   alib.log.Put ("MMSG %p: Reading msg @%ld (max=%ld), txt=%p, maxctrl=%ld, ctrl=%p", this, StartOfs, MaxSize, Txt, MaxCtrlSize, Ctrl);
#endif
   if (MsgReadMsg (handle, &header, StartOfs, MaxSize, Txt, MaxCtrlSize, Ctrl) == 0)
   {
      alib.log.Put ("!Error #%d reading message", msgapierr);
      return 0;
   }

   return 1;
}

int Message::Write (word Append, byte *Txt, byte *Ctrl)
{
int lt=0, lc=0;

   if (Txt)
      lt = strlen (Txt) + 1;
   if (Ctrl)
      lc = strlen (Ctrl) + 1;

   Alert (handle == NULL);

#ifdef DEBUG
   alib.log.Put ("MMSG %p: Writing msg (append=%s, txt=%p, ctrl=%p)", this, (Append ? "Yes" : "No"), Txt, Ctrl);
#endif
   if (MsgWriteMsg (handle, Append, &header, Txt, lt, lt, lc, Ctrl) == -1)
   {
      alib.log.Put ("!Error #%d writing to message", msgapierr);
      return 0;
   }

   return 1;
}

dword Message::GetTextLen (void)
{
   Alert (handle == NULL);
   return (MsgGetTextLen (handle));
}

dword Message::GetCtrlLen (void)
{
   Alert (handle == NULL);
   return (MsgGetCtrlLen (handle));
}

UMSGID Message::MsgnToUID (dword MsgNum)
{
   Alert (handle == NULL);

   return (MsgMsgnToUid (AreaHandle, MsgNum));
}

dword Message::UIDToMsgn (word Type, UMSGID msgid)
{
   Alert (handle == NULL);
   return (MsgUidToMsgn (AreaHandle, msgid, Type));
}

byte *Message::GetCtrlToken (byte *Buffer, byte *Tag)
{
   Alert (Buffer == NULL);
   Alert (Tag == NULL);

   return (MsgGetCtrlToken (Buffer, Tag));
}

void Message::FreeCtrlToken (byte *CtrlToken)
{
   MsgFreeCtrlToken (CtrlToken);
}

void Message::RemoveCtrlToken (byte *Buffer, byte *Tag)
{
   Alert (Buffer == NULL);
   Alert (Tag == NULL);

   MsgRemoveToken (Buffer, Tag);
}

word Message::GetNumKludges (byte *Buffer)
{
   Alert (Buffer == NULL);

   return (MsgGetNumKludges (Buffer));
}

dword Message::SQ_Hash (byte *Buffer)
{
   Alert (Buffer == NULL);

   return (SquishHash (Buffer));
}
