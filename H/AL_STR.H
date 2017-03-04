// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
// AL_STR.H: ALIB; String declarations                                    //
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
#ifndef __AL_STR_H
#define __AL_STR_H

#define NUL    '\0'
void strtrim (char *str);
void kill_filename_trail_slash (char *str);
void add_filename_trail_slash (char *str);

void parsenetaddr (NETADDR &pa, char *pstr);
void commafmt (char *buffer, int bufsz, long number);
char *wordwrap (char *string, size_t line_len);


#define P2Cconvert(s) do \
            {unsigned char n = *(s); memmove((s), &(s)[1], n); s[n] = '\0';} while(0)
#define C2Pconvert(s) do\
            {int n = strlen(s); memmove(&(s)[1], (s), n); *(s) = n;} while(0)

#endif

