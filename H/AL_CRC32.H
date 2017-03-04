// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
// AL_CRC32.H: ALIB; CRC-32 calculations                                  //
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
#ifndef __AL_CRC32_H
#define __AL_CRC32_H

#define UPDCRC32(octet,crc) ((long)crc_32_tab[((crc)^((byte)octet)) & 0xff] ^ ((crc) >> 8))

dword updateCRC32(unsigned char ch, dword crc);
int crc32file(char *name, dword *crc, long *charcnt);
dword crc32buf(char *buf, size_t len);
extern dword crc_32_tab[];
#define xcrc(crc,cp) ( crctab16[((crc >> 8) & 255) ^ cp] ^ (crc << 8))
extern unsigned short crctab16[];

#endif
