// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
// ALIB.H: ALIB; General declarations used in ALIB                        //
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //

#ifndef __AL_fOS_H
#define __AL_fOS_H

#define BAUD_300		0x040
#define BAUD_1200		0x080
#define BAUD_2400		0x0A0
#define BAUD_4800		0x0C0
#define BAUD_9600		0x0E0
#define BAUD_19200		0x000
#define BAUD_38400		0x020

#define EXTENDED_BAUDRATES
#define BAUD_EXT_110	0x00
#define BAUD_EXT_150    0x01
#define BAUD_EXT_300    0x02
#define BAUD_EXT_600    0x03
#define BAUD_EXT_1200   0x04
#define BAUD_EXT_2400   0x05
#define BAUD_EXT_4800   0x06
#define BAUD_EXT_9600   0x07
#define BAUD_EXT_19200  0x08
#define BAUD_EXT_28800  0x80
#define BAUD_EXT_38400  0x81
#define BAUD_EXT_57600  0x82
#define BAUD_EXT_76800  0x83 /* With standard UART this isn't possible */
#define BAUD_EXT_115200 0x84

#define BITS_7			0x02
#define BITS_8			0x03
#define STOP_1			0x00
#define STOP_2			0x04
#define ODD_PARITY		0x08
#define EVEN_PARITY		0x18
#define NO_PARITY		0x00

/* Bit definitions for the driver flags */

#define USE_XON			0x01
#define USE_CTS			0x02
#define USE_DSR			0x04
#define OTHER_XON		0x08

#define BRK				0x01
#define MDM				0x02

unsigned Com_ (char, byte);
int modem_status (void);
extern word handshake_mask;

struct FOS_LINE_CONTROL
{
	int carrier_mask;
	int handshake_mask;
};

struct baud_str
{
	unsigned long rate_value;
	unsigned long rate_mask;
};

struct parse_list
{
	int p_length;
	char p_string[20];
};

typedef struct finfo
{
	int info_size;
	char curr_fossil;
	char curr_rev;
	char far *id_string;
	int ibufr;
	int ifree;
	int obufr;
	int ofree;
	byte swidth;
	byte sheight;
	char baud;
} FOSINFO;

/* Bit definitions for the Line Status Register */

#define DATA_READY		0x0100
#define OVERRUN_ERR		0x0200
#define PARITY_ERR		0x0400
#define FRAMING_ERR		0x0800
#define BREAK_INT		0x1000
#define TX_HOLD_EMPTY	0x2000
#define TX_SHIFT_EMPTY	0x4000

class Fossil
{
private:
   int Port;
   int old_fossil;
   int fossil_count;
   int out_count;

   void  fill_buffer();
public:
//   struct FOS_LINE_CONTROL line;
   word  comm_bits;
   word  parity;
   word  stop_bits;
   word  carrier_mask;

   char  fossil_buffer [256];
   char  out_buffer [256];

   char *fossil_fetch_pointer;
   char *out_send_pointer;

   int GetFosCount (void) {return fossil_count;}
   dword Xfered;

   Fossil() {Port = old_fossil = fossil_count = out_count = 0;
             fossil_fetch_pointer = fossil_buffer;
             out_send_pointer = out_buffer;
             comm_bits = BITS_8;
             parity = NO_PARITY;
             stop_bits = STOP_1;
             carrier_mask = 0x80;
             handshake_mask = USE_XON|USE_CTS;
             Xfered=0L;}

  ~Fossil() {mdm_disable();}
   void SetPort (int p) {Port = p;}
   int  GetPort (void) {return Port;}
   FOSINFO fossil_info;

   void Fossil::fossil_ver();
   void Fossil::com_kick ();

   int com_getc (int);
   unsigned Cominit (int, int);
   void mdm_disable (void);
   void mdm_enable (unsigned long);
   int modem_in (void);
   int peekbyte (void);
   void sendbyte (unsigned char);
   void clear_inbound (void);
   void clear_outbound (void);
   void sendchars (char far *, unsigned int, int);
   void buffer_byte (unsigned char);
   void unbuffer_bytes (void);
   void do_break (int);
//   word ComTXBlockTimeout (char *lpBuf, word cbBuf, dword ulTimer);
//   word ComTXRemain (void);
};

/* CPS rate monitor. */
class CPS_Rate
{
private:
   int      Cps [8];                   /* Last 8 values. */
   int      Values;
   time_t   Start;
   time_t   LastSample;
   int      getAverageCps();

public:
   CPS_Rate();
   void     begin (void);
   void     sample (void);
   operator int () {return getAverageCps();}
};

/*--------------------------------------------------------------------------*/
/* MACROS that call the routines in COM_ASM                                 */
/*--------------------------------------------------------------------------*/
//extern unsigned Com_ (char, byte);
//extern unsigned Cominit (int, int);
//extern int READBYTE ();

/*#define M_INSTALL(b)       {Cominit(com_port);Com_(0x0f,0);Com_(0x0f,handshake_mask);new_baud(0,b);}*/

/*-----------------------------------------------*/
/* Service 0: SET BAUD(etc)                      */
/*-----------------------------------------------*/

/*#define MDM_ENABLE(b)      (Com_(0x00,b|NO_PARITY|STOP_1|BITS_8))*/

/*-----------------------------------------------*/
/* Service 1: SEND CHAR (wait)                   */
/*-----------------------------------------------*/

/*#define SENDBYTE(x)    (Com_(0x01,x))*/

/*-----------------------------------------------*/
/* Service 2: GET CHAR (wait)                    */
/*-----------------------------------------------*/

/*#define MODEM_IN()         (Com_(0x02)&0x00ff)*/

/*-----------------------------------------------*/
/* Service 3: GET STATUS                         */
/*-----------------------------------------------*/

/*#define MODEM_STATUS()     (Com_(0x03))*/
#define CARRIER            (Com_(0x03,0)&0x80)
#define CHAR_AVAIL()       (modem_status()&DATA_READY)
#define OUT_EMPTY()        (Com_(0x03,0)&TX_SHIFT_EMPTY)
#define OUT_FULL()         (!(Com_(0x03,0)&TX_HOLD_EMPTY))
int READBYTE();

/*-----------------------------------------------*/
/* Service 4: INIT/INSTALL                       */
/*-----------------------------------------------*/

/*-----------------------------------------------*/
/* Service 5: UNINSTALL                          */
/*-----------------------------------------------*/

/*#define MDM_DISABLE()      (Com_(0x05,BAUD_2400|NO_PARITY|STOP_1|BITS_8))*/

/*-----------------------------------------------*/
/* Service 6: SET DTR                            */
/*-----------------------------------------------*/

#define LOWER_DTR()        ((void) Com_(0x06,0))
#define RAISE_DTR()        ((void) Com_(0x06,1))

/*-----------------------------------------------*/
/* Service 7: GET TIMER TICK PARMS               */
/*-----------------------------------------------*/

/*-----------------------------------------------*/
/* Service 8: FLUSH OUTBOUND RING-BUFFER         */
/*-----------------------------------------------*/

/*-----------------------------------------------*/
/* Service 9: NUKE OUTBOUND RING-BUFFER          */
/*-----------------------------------------------*/

/*#define CLEAR_OUTBOUND()   (Com_(0x09,0))*/

/*-----------------------------------------------*/
/* Service a: NUKE INBOUND RING-BUFFER           */
/*-----------------------------------------------*/

/*#define CLEAR_INBOUND()    (Com_(0x0a,0))*/

/*-----------------------------------------------*/
/* Service b: SEND CHAR (no wait)                */
/*-----------------------------------------------*/

#define Com_Tx_NW(c)       (Com_(0x0b,c))

/*-----------------------------------------------*/
/* Service c: GET CHAR (no wait)                 */
/*-----------------------------------------------*/

/*#define PEEKBYTE()         (Com_(0x0c,0))*/

/*-----------------------------------------------*/
/* Service d: GET KEYBOARD STATUS                */
/*-----------------------------------------------*/

#define KEYPRESS()         (Com_(0x0d,0)!=(0xffff))
#define FOSSIL_PEEKKB()    (Com_(0x0d,0))

/*-----------------------------------------------*/
/* Service e: GET KEYBOARD CHARACTER (wait)      */
/*-----------------------------------------------*/

/* #define READKB()           (Com_(0x0e,0)&0xff) */
#define READSCAN()         (Com_(0x0e,0))

#define FOSSIL_CHAR()      READSCAN()

/*-----------------------------------------------*/
/* Service f: SET/GET FLOW CONTROL STATUS        */
/*-----------------------------------------------*/

#define XON_ENABLE()       ((void) Com_(0x0f,(byte)handshake_mask))
#define XON_DISABLE()      ((void) Com_(0x0f,(byte)(handshake_mask&(~USE_XON))))
#define IN_XON_ENABLE()    ((void) Com_(0x0f,(byte)(handshake_mask|OTHER_XON)))
#define IN_XON_DISABLE()   ((void) Com_(0x0f,(byte)(handshake_mask&(~OTHER_XON))))

/*-----------------------------------------------*/
/* Service 10: SET/GET CTL-BREAK CONTROLS        */
/*             Note that the "break" here refers */
/*             to ^C and ^K rather than the      */
/*             tradition modem BREAK.            */
/*-----------------------------------------------*/

#define _BRK_ENABLE()      (Com_(0x10,BRK))
#define _BRK_DISABLE()     (Com_(0x10,0))
#define RECVD_BREAK()      (Com_(0x10,BRK)&BRK)

/*-----------------------------------------------*/
/* Service 11: SET LOCAL VIDEO CURSOR POSITION   */
/*-----------------------------------------------*/

/*-----------------------------------------------*/
/* Service 12: GET LOCAL VIDEO CURSOR POSITION   */
/*-----------------------------------------------*/

/*-----------------------------------------------*/
/* Service 13: WRITE LOCAL ANSI CHARACTER        */
/*-----------------------------------------------*/

#define WRITE_ANSI(c)    ((void) Com_(0x13,(byte)(c)))

/*-----------------------------------------------*/
/* Service 14: WATCHDOG on/off                   */
/*-----------------------------------------------*/

#define FOSSIL_WATCHDOG(x) (Com_(0x14,x))

/*-----------------------------------------------*/
/* Service 15: BIOS write to screen              */
/*-----------------------------------------------*/

#define WRITE_BIOS(c) (Com_(0x15,(byte)c))

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* A no-stall ReadByte routine might look like this:                        */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#endif

