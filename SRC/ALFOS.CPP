#include <alib.h>
Set_Alert_File ("ALFOS.CPP<library ALIB>")

Fossil Modem;

int FailSafeTimer = 0;
word  handshake_mask;

void Fossil::fossil_ver ()
{
	union REGS r;
	struct SREGS s;

	char far *q;

	q = (char far *) &fossil_info;

	r.x.cx = sizeof (struct finfo);

	segread (&s);
	s.es = s.ds = FP_SEG (q);

	r.x.di = r.x.si = FP_OFF (q);

	r.x.dx = Port;
	r.x.ax = 0x1b00;

	(void) int86x (0x14, &r, &r, &s);

   if (fossil_info.curr_fossil > 0)
		old_fossil = 0;
}

void Fossil::com_kick ()
{
	XON_DISABLE ();				/* Uncork the transmitter */
	XON_ENABLE ();
}

unsigned int Com_ (char request, byte parm1)
{
	union REGS r;
      
	r.h.ah = request;
	r.h.al = parm1;
   r.x.dx = Modem.GetPort();

	return int86 (0x14, &r, &r);
}

unsigned Fossil::Cominit (int WhichPort, int failsafe)
{
	union REGS r;
	struct SREGS s;
	int ret;

	fossil_fetch_pointer = fossil_buffer;
	fossil_count = 0;

	r.h.ah = 4;
   r.x.dx = WhichPort;
   SetPort (WhichPort);
   ret = int86 (0x14, &r, &r);

	if (ret == 0x1954)
	{
		FailSafeTimer = failsafe;
		r.x.ax = 0x0f01;
		r.x.dx = Port;
		(void) int86 (0x14, &r, &r);
      fossil_ver();
	}
	return (ret);
}

int Fossil::com_getc (int t)
{
   Timer t1;

	if (!CHAR_AVAIL ())
	{
      t1.set (t);

		while (!CHAR_AVAIL ())
		{
         if ((int)t1)
			{
				return (EOF);
			}

			/*
          * This should work because we only do TIMED_READ when we have
          * carrier
          */
			if (!CARRIER)
			{
				return (EOF);
			}
         os.Slice();
		}
	}
   return ((unsigned int) (modem_in ()) & 0x00ff);
}

int Fossil::modem_in (void)
{
	unsigned char c;

	while (fossil_count == 0)
	{
		fill_buffer ();
		if (fossil_count == 0)
         os.Slice();
	}
	--fossil_count;
	c = (unsigned char) *fossil_fetch_pointer++;
   Xfered++;
	return ((int) c);
}

int Fossil::peekbyte (void)
{
	unsigned char c;

	if (fossil_count == 0)
	{
		fill_buffer ();
		if (fossil_count == 0)
			return (-1);
	}
	c = (unsigned char) *fossil_fetch_pointer;
	return ((int) c);
}

void Fossil::fill_buffer (void)
{
	union REGS r;
	struct SREGS s;
	char far *ptr = (char far *) fossil_buffer;
	int ret;

	if (fossil_count != 0)
		return;

	fossil_fetch_pointer = fossil_buffer;

	if (old_fossil == 0)
	{
      r.x.cx = 255;
		r.x.dx = Port;

		segread (&s);

		r.x.di = r.x.si = FP_OFF (ptr);
		s.es = s.ds = FP_SEG (ptr);

		r.x.ax = 0x1800;

		fossil_count = int86x (0x14, &r, &r, &s);
	}
	else
	{
		r.x.ax = 0x0c00;
		r.x.dx = Port;

		ret = int86 (0x14, &r, &r);

		if (ret != -1)
		{
			fossil_count = 1;
			*fossil_fetch_pointer = (char) ret;
			r.x.ax = 0x0200;
			(void) int86 (0x14, &r, &r);
		}
	}
}

void Fossil::clear_inbound (void)
{
	union REGS r;

	fossil_fetch_pointer = fossil_buffer;
	fossil_count = 0;

	r.x.ax = 0x0a00;
	r.x.dx = Port;

	(void) int86 (0x14, &r, &r);
}

void Fossil::clear_outbound (void)
{
	union REGS r;

	out_send_pointer = out_buffer;
	out_count = 0;

	r.x.ax = 0x0900;
   r.x.dx = GetPort();

	(void) int86 (0x14, &r, &r);
}

int modem_status (void)
{
	union REGS r;
	int ret;

	r.x.ax = 0x0300;
   r.x.dx = Modem.GetPort();

	ret = int86 (0x14, &r, &r);

   if (Modem.GetFosCount() != 0)
		ret |= 256;

	return (ret);
}

void Fossil::do_break (int on_off)
{
	union REGS r;

	r.h.al = (char) on_off;
	r.h.ah = 0x1a;
   r.x.dx = GetPort();

	(void) int86 (0x14, &r, &r);
}

void Fossil::mdm_enable (unsigned long mask)
{
	union REGS r;

   clear_inbound ();

	r.h.al = (byte) (mask | comm_bits | parity | stop_bits);
	r.h.ah = 0;
   r.x.dx = GetPort();

	(void) int86 (0x14, &r, &r);
}

void Fossil::mdm_disable (void)
{
	union REGS r;

	fossil_fetch_pointer = fossil_buffer;
	fossil_count = 0;

	r.x.ax = 0x0500;
   r.x.dx = GetPort();

	(void) int86 (0x14, &r, &r);
}

void Fossil::unbuffer_bytes (void)
{
	if (out_count == 0)
		return;

   sendchars ((char far *) out_buffer, out_count, 1);

	out_send_pointer = out_buffer;
	out_count = 0;
}

void Fossil::buffer_byte (unsigned char c)
{
   if (out_count == 255)
		unbuffer_bytes ();

	out_count++;
	*out_send_pointer++ = c;
}

void Fossil::sendbyte (unsigned char c)
{
	union REGS r;
	int done = 0;

	if (out_count != 0)
		unbuffer_bytes ();

	while (done == 0)
	{
		r.h.ah = 0x0b;
		r.h.al = c;
      r.x.dx = GetPort();
      Xfered++;

		done = int86 (0x14, &r, &r);
		if (done == 0)
		{
			(void) peekbyte ();	/* Waiting for output, try to get input */
         os.Slice();
		}
	}
}

void Fossil::sendchars (char far * str, unsigned len, int dcd)
{
	union REGS r1, r2;
	struct SREGS s;
   Timer timer;
	int sent;
   int timerset=0;

	if (old_fossil)
	{
		r1.x.ax = 0x0b00;
      r1.x.dx = GetPort();

		while (len != 0)
		{
			r1.h.al = *str;
			if (int86 (0x14, &r1, &r2) != 0)
			{
				str++;
				len--;
			}
			else
			{
				if (FailSafeTimer)
				{
               if (!timerset)
               {
                  timerset=1;
                  timer.set (FailSafeTimer);
               }
					else
					{
                  if (!timer)
						{
							clear_outbound ();
							LOWER_DTR ();
							return;
						}
					}
				}

				(void) peekbyte ();	/* Waiting for output, try to get input */
            os.Slice();
				if (dcd && !CARRIER)
					return;
			}
		}
		return;
	}

	r1.x.ax = 0x1900;
   r1.x.dx = GetPort();

	segread (&s);

	s.es = s.ds = FP_SEG (str);

	while (len != 0)
	{
		r1.x.cx = len;
      Xfered += len;
      r1.x.si = r1.x.di = FP_OFF (str);
		sent = int86x (0x14, &r1, &r2, &s);
		if (sent == (int) len)
			return;

		len -= sent;
		str += sent;

		if (FailSafeTimer)
		{
         if (!timerset)
         {
            timerset = 1;
            timer.set (FailSafeTimer);
         }
			else
			{
            if (!timer)
				{
					clear_outbound ();
					LOWER_DTR ();
					return;
				}
			}
		}

		(void) peekbyte ();		/* Waiting for output, try to get input */
      os.Slice();
		if (dcd && !CARRIER)
			return;
	}
}

/* Currently only used by HYDRA */

/*
 * This is a lot like ComWrite, but will return the sent-character
 * count if either a timeout occurs or carrier is lost. The timer
 * is specified by passing in a time_t for end-of-time. If zero is
 * passed for the timer, we'll just check for carrier loss.
 *

USHORT
ComTXBlockTimeout (BYTE *lpBuf, USHORT cbBuf, ULONG ulTimer)
{
	USHORT cbRemaining = cbBuf;
	USHORT cbToSend;
	BOOL fTimedOut;

	fTimedOut = (ulTimer != 0) ? (ulTimer < (ULONG)time (NULL)) : FALSE;
	while (!fTimedOut && CARRIER && cbRemaining)
	{
		cbToSend = ComOutSpace ();
		if ((--cbToSend) > 0)
		{
			if (cbToSend > cbRemaining)
				cbToSend = cbRemaining;
			sendchars (lpBuf, cbToSend, 1);
			lpBuf += cbToSend;
			cbRemaining -= cbToSend;
		}
		else
         os.Slice();
		fTimedOut = (ulTimer != 0) ? (ulTimer < (ULONG)time (NULL)) : FALSE;
	}
	return (cbBuf - cbRemaining);
}

USHORT
ComTXRemain (void)
{
	union REGS r1, r2;
	struct SREGS s;
	char far *q;

	if (old_fossil)
		return (0);

	q = (char far *) &fossil_info;

	r1.x.ax = 0x1b00;
	r1.x.cx = sizeof (FOSINFO);
   r1.x.dx = GetPort();
	segread(&s);
	s.es = s.ds = FP_SEG (q);
	r1.x.di = FP_OFF (q);
	(void) int86x (0x14,&r1,&r2,&s);

	return (fossil_info.obufr - fossil_info.ofree);
}

static USHORT
ComOutSpace (void)
{
	union REGS r1, r2;
	struct SREGS s;
	char far *q;

	if (old_fossil)
		return (1);

	q = (char far *) &fossil_info;

	r1.x.ax = 0x1b00;
	r1.x.cx = sizeof (FOSINFO);
   r1.x.dx = GetPort();
	segread(&s);
	s.es = s.ds = FP_SEG (q);
	r1.x.di = FP_OFF (q);
	(void) int86x (0x14,&r1,&r2,&s);

	return (fossil_info.ofree);
}


void 
fossil_gotoxy (int col, int row)
{
	union REGS r;

	r.h.dh = (char) row;
	r.h.dl = (char) col;
	r.x.ax = 0x1100;

	(void) int86 (0x14, &r, &r);
}

int 
fossil_whereami (void)
{
	union REGS r;

	r.x.ax = 0x1200;
	(void) int86 (0x14, &r, &r);
	return r.x.dx;
}

int 
fossil_wherey (void)
{
	union REGS r;

	r.x.ax = 0x1200;
	(void) int86 (0x14, &r, &r);
	return (int) r.h.dh;
}

int 
fossil_wherex (void)
{
	union REGS r;

	r.x.ax = 0x1200;
	(void) int86 (0x14, &r, &r);
	return (int) r.h.dl;
}
*/

int READBYTE()
{                                                             
   return( CHAR_AVAIL() ? Modem.modem_in() : (-1) );                
}                                                             

/*************************************************************************/
/* CPS RATES                                                             */
/*************************************************************************/
CPS_Rate::CPS_Rate()
{
}

void CPS_Rate::begin (void)
{
   Start = time (NULL);
   LastSample = 0L;
   for (Values=0; Values < 8; Values++)
      Cps[Values] = 0;
   Values=0;
   Modem.Xfered = 0L;

}

void CPS_Rate::sample (void)
{
time_t now;
time_t dif;
int v;

   now = time(NULL);

   if (!LastSample)
      LastSample = Start;

   dif = now - LastSample;
   if (dif < 3)
      return;

   LastSample = now;

   Cps[0] = (int)(Modem.Xfered / dif);
   Modem.Xfered = 0L;
}

int CPS_Rate::getAverageCps()
{
   return (Cps[0]);
}
