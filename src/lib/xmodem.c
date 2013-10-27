#include <string.h>
#include <stdio.h>
#include <lib/xmodem.h>
#include <stdint.h>

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 100000
#define MAXRETRANS 25


static int (*_writer)(unsigned char* buffer, int size);

void xmodem_init(int (*writer)(unsigned char* buffer, int size))
{
	_writer = writer;
}


unsigned short crc16(const unsigned char *buf, int sz)
{
        unsigned short crc = 0;
        while (--sz >= 0)
        {
                int i;
                crc ^= (unsigned short) *buf++ << 8;
                for (i = 0; i < 8; i++)
                        if (crc & 0x8000)
                                crc = crc << 1 ^ 0x1021;
			else
				crc <<= 1;
        }
        return crc;
}


static int check(int crc, const unsigned char *buf, int sz)
{
	if (crc)
	{
		unsigned short crc = crc16(buf, sz);
		unsigned short tcrc = (buf[sz]<<8)+buf[sz+1];
		if (crc == tcrc)
			return 1;
	}
	else
	{
		int i;
		unsigned char cks = 0;
		for (i = 0; i < sz; ++i)
		{
			cks += buf[i];
		}
		if (cks == buf[sz])
			return 1;
	}

	return 0;
}

static void flushinput(void)
{
	while (try_getc() >= 0);
}

int xmodem_get(unsigned char *dest, int destsz)
{
	unsigned char xbuff[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	unsigned char *p;
	int bufsz, crc = 0;
	unsigned char trychar = 'C';
	unsigned char packetno = 1;
	int i, c, len = 0;
	int retry, retrans = MAXRETRANS;

	for (;;)
	{
		for ( retry = 0; retry < 160; ++retry)
		{
			if (trychar)
			{
				putchar(trychar);
				//      sleep(0x008f0000);
			}
			if ((c = try_getc((DLY_1S)<<1)) >= 0)
			{
				switch (c)
				{
				case SOH:
					bufsz = 128;
					goto start_recv;
				case STX:
					bufsz = 1024;
					goto start_recv;
				case EOT:
					flushinput();
					putchar(ACK);
					return len; /* normal end */
				case CAN:
					if ((c = try_getc(DLY_1S)) == CAN)
					{
						flushinput();
						putchar(ACK);
						return -1; /* canceled by remote */
					}
					break;
				default:
					break;
				}
			}
		}
		if (trychar == 'C')
		{
			trychar = NAK;
			continue;
		}
		flushinput();
		putchar(CAN);
		putchar(CAN);
		putchar(CAN);
		return -2; /* sync error */

	start_recv:
		if (trychar == 'C') crc = 1;
		trychar = 0;
		p = xbuff;
		*p++ = c;
		for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i)
		{
			if ((c = try_getc(DLY_1S)) < 0) goto reject;
			*p++ = c;
		}

		if (xbuff[1] == (unsigned char)(~xbuff[2]) &&
		    (xbuff[1] == packetno || xbuff[1] == (unsigned char)packetno-1) &&
		    check(crc, &xbuff[3], bufsz))
		{
			if (xbuff[1] == packetno)
			{
				unsigned int count = destsz - len;
				if (count > bufsz) count = bufsz;
				if (count > 0)
				{
					//memcpy (&dest[len], &xbuff[3], count);
					_writer(&xbuff[3], count);
					len += count;
				}
				++packetno;
				retrans = MAXRETRANS+1;
			}
			if (--retrans <= 0)
			{
				flushinput();
				putchar(CAN);
				putchar(CAN);
				putchar(CAN);
				return -3; /* too many retry error */
			}
			putchar(ACK);
			continue;
		}
	reject:
		flushinput();
		putchar(NAK);
	}
}
/*
  int xmodemTransmit(unsigned char *src, int srcsz)
  {
  //unsigned char xbuff[1030]; // 1024 for XModem 1k + 3 head chars + 2 crc + nul 
  int bufsz, crc = -1;
  unsigned char packetno = 1;
  int i, c, len = 0;
  int retry;

  for (;;)
  {
  for ( retry = 0; retry < 16; ++retry)
  {
  if ((c = dbgu_getc_nonblock((DLY_1S)<<1)) >= 0)
  {
  switch (c)
  {
  case 'C':
  crc = 1;
  goto start_trans;
  case NAK:
  crc = 0;
  goto start_trans;
  case CAN:
  if ((c = dbgu_getc_nonblock(DLY_1S)) == CAN)
  {
  putchar(ACK);
  flushinput();
  return -1; // canceled by remote 
  }
  break;
  default:
  break;
  }
  }
  }
  putchar(CAN);
  putchar(CAN);
  putchar(CAN);
  flushinput();
  return -2; // no sync 

  for (;;)
  {
  start_trans:
  xbuff[0] = SOH;
  bufsz = 128;
  xbuff[1] = packetno;
  xbuff[2] = ~packetno;
  c = srcsz - len;
  if (c > bufsz) c = bufsz;
  if (c >= 0)
  {
  memset (&xbuff[3], 0, bufsz);
  if (c == 0)
  {
  xbuff[3] = CTRLZ;
  }
  else
  {
  memcpy (&xbuff[3], &src[len], c);
  if (c < bufsz) xbuff[3+c] = CTRLZ;
  }
  if (crc)
  {
  unsigned short ccrc = crc16(&xbuff[3], bufsz);
  xbuff[bufsz+3] = (ccrc>>8) & 0xFF;
  xbuff[bufsz+4] = ccrc & 0xFF;
  }
  else
  {
  unsigned char ccks = 0;
  for (i = 3; i < bufsz+3; ++i)
  {
  ccks += xbuff[i];
  }
  xbuff[bufsz+3] = ccks;
  }
  for (retry = 0; retry < MAXRETRANS; ++retry)
  {
  for (i = 0; i < bufsz+4+(crc?1:0); ++i)
  {
  putchar(xbuff[i]);
  }
  if ((c = dbgu_getc_nonblock(DLY_1S)) >= 0 )
  {
  switch (c)
  {
  case ACK:
  ++packetno;
  len += bufsz;
  goto start_trans;
  case CAN:
  if ((c = dbgu_getc_nonblock(DLY_1S)) == CAN)
  {
  putchar(ACK);
  flushinput();
  return -1; // canceled by remote 
  }
  break;
  case NAK:
  default:
  break;
  }
  }
  }
  putchar(CAN);
  putchar(CAN);
  putchar(CAN);
  flushinput();
  return -4; // xmit error 
  }
  else
  {
  for (retry = 0; retry < 10; ++retry)
  {
  putchar(EOT);
  if ((c = dbgu_getc_nonblock((DLY_1S)<<1)) == ACK) break;
  }
  flushinput();
  return (c == ACK)?len:-5;
  }
  }
  }
  }*/
