// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
// AL_JAM.H: ALIB; JAM support                                            //
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
#ifndef __AL_JAM_H
#define __AL_JAM_H

typedef struct
{
   char     signature [4];          // (JAM<NUL>)
   time_t   creation_date;
   dword    modcount;
   dword    activemsgs;
   dword    passwordcrc;
   dword    basemsgnum;
   char     reserved [1000];
} JAMFixedHeader;

typedef struct
{
   char     signature [4];          // (JAM<NUL>)
   word     revision;
   word     reserved;
   dword    SubfieldLen;
   dword    TimesRead;
   dword    MSGIDcrc;
   dword    REPLYcrc;
   dword    replyto;
   dword    reply1st;
   dword    replynext;
   time_t   datewritten;
   time_t   daterecvd;
   time_t   dateprocessed;
   dword    msgnum;
   dword    attr1;
   dword    attr2;
   dword    offset;
   dword    txtlen;
   dword    PasswordCRC;
   dword    cost;
} JAMMsgHeader;

typedef struct
{
   word        LoID;
   word        HiID;
   dword       datlen;
//  char        *Buffer;       
} JAMSubfield;

typedef struct JAMSubFieldlst
{
   word        Lo, Hi;
   dword       datlen;
   char        *Buffer;
   struct JAMSubFieldlst *Next;
} JAMSubFieldList;

typedef struct
{
   dword       Crc;                 // Lowercase crc-32 of recipient's name
   dword       ofs;                 // Offset into .JHR file.
} JAMIndexRec;

// JAMSubfield::LoID fields
#define JSUB_OAddr               0
#define JSUB_DAddr               1
#define JSUB_From                2
#define JSUB_To                  3
#define JSUB_MSGID               4
#define JSUB_REPLYID             5
#define JSUB_Subject             6
#define JSUB_PID                 7
#define JSUB_Trace               8              // ^AVia
#define JSUB_EnclosedFile        9
#define JSUB_EnclosedFileAlias   10
#define JSUB_EnclosedFreq        11
#define JSUB_EnclosedFileWild    12
#define JSUB_EnclosedIndirectFile   13
#define JSUB_EMBINDAT            1000
#define JSUB_FTSKludge           2000
#define JSUB_Seendby2D           2001
#define JSUB_Path2D              2002
#define JSUB_Flags               2003
#define JSUB_Timezone_Info       2004              // [+/-]???? (i.e. +0500)

// Message attributes
#define JMSG_LOCAL       0x00000001L    /* Msg created locally */
#define JMSG_INTRANSIT   0x00000002L    /* Msg is in-transit */
#define JMSG_PRIVATE     0x00000004L    /* Private */
#define JMSG_READ        0x00000008L    /* Read by addressee */
#define JMSG_SENT        0x00000010L    /* Sent to remote */
#define JMSG_KILLSENT    0x00000020L    /* Kill when sent */
#define JMSG_ARCHIVESENT 0x00000040L    /* Archive when sent */
#define JMSG_HOLD        0x00000080L    /* Hold for pick-up */
#define JMSG_CRASH       0x00000100L    /* Crash */
#define JMSG_IMMEDIATE   0x00000200L    /* Send Msg now, ignore restrictions */
#define JMSG_DIRECT      0x00000400L    /* Send directly to destination */
#define JMSG_GATE        0x00000800L    /* Send via gateway */
#define JMSG_FILEREQUEST 0x00001000L    /* File request */
#define JMSG_FILEATTACH  0x00002000L    /* File(s) attached to Msg */
#define JMSG_TRUNCFILE   0x00004000L    /* Truncate file(s) when sent */
#define JMSG_KILLFILE    0x00008000L    /* Delete file(s) when sent */
#define JMSG_RECEIPTREQ  0x00010000L    /* Return receipt requested */
#define JMSG_CONFIRMREQ  0x00020000L    /* Confirmation receipt requested */
#define JMSG_ORPHAN      0x00040000L    /* Unknown destination */
#define JMSG_ENCRYPT     0x00080000L    /* Msg text is encrypted */
#define JMSG_COMPRESS    0x00100000L    /* Msg text is compressed */
#define JMSG_ESCAPED     0x00200000L    /* Msg text is seven bit ASCII */
#define JMSG_FPU         0x00400000L    /* Force pickup */
#define JMSG_TYPELOCAL   0x00800000L    /* Msg is for local use only (not for export) */
#define JMSG_TYPEECHO    0x01000000L    /* Msg is for conference distribution */
#define JMSG_TYPENET     0x02000000L    /* Msg is direct network mail */
#define JMSG_NODISP      0x20000000L    /* Msg may not be displayed to user */
#define JMSG_LOCKED      0x40000000L    /* Msg is locked, no editing possible */
#define JMSG_DELETED     0x80000000L    /* Msg is deleted */

int JamCreateBase (char *path);
int JamOpenBase (char *path);
void JamCloseBase (void);
void JamInitMsgWrite (JAMMsgHeader *hdr);
void JamAddSubfield (word field, char *buffer);
int JamSaveMessage (JAMMsgHeader *hdr, char *msgtext);
dword JAMCrc32(void far *pBuf, word len, dword crc);

#endif
