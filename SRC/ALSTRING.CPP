// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 
// ALSTRING.CPP: ALIB; String functions
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// Implements various string handling functions, including:
//    strtrim
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 

#include <alib.h>
Set_Alert_File ("ALSTRING.CPP<library ALIB>")

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void strtrim (char *str)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *str          String to trim
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Removes trailing, multiple embedded, and leading spaces.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void strtrim (char *str)
{
   Alert (str == NULL);
   Alert (*str == NUL);

char *ibuf, *obuf;

   for (ibuf = obuf = str; *ibuf; )
   {
      while (*ibuf && (isspace (*ibuf)))
         ibuf++;
      if (*ibuf && (obuf != str))
         *(obuf++) = ' ';
      while (*ibuf && (!isspace (*ibuf)))
         *(obuf++) = *(ibuf++);
   }
   *obuf = '\0';
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void kill_filename_trail_slash (char *str)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *str          String to operate on
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Removes a trailing '\' from a filename/path if one is there.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void kill_filename_trail_slash (char *str)
{
char *j = str;
   Alert (str == NULL);
   Alert (*str == NUL);

   while (*j)
      j++;

   j--;
   if (*j == '\\')
      *j = NUL;
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void add_filename_trail_slash (char *str)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *str          String to operate on
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Adds a trailing '\' from a filename/path if one is not there.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void add_filename_trail_slash (char *str)
{
char *j = str;
   Alert (str == NULL);
   Alert (*str == NUL);

   while (*j)
      j++;

   j--;
   if (*j != '\\')
   {
      j++;
      *j = '\\';
      j++;
      *j = NUL;
   }
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void parsenetaddr (NETADDR &pa, char *pstr)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: NETADDR *pa        Pointer to NETADDR string
//            char *pstr         Pointer to string version
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Parses a net address in pstr
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void parsenetaddr (NETADDR &pa, char *pstr)
{
   pa.zone = pa.net = pa.node = pa.point = 0;
   if (sscanf (pstr, "%u:%u/%u.%u", &pa.zone, &pa.net, &pa.node,
                                    &pa.point) < 3)
   {
      pa.zone = 0;
      return;
   }
   if (pa.zone > 4095 || pa.net > 32766 || pa.node == 32766 ||
       pa.point > 32766)
   {
      pa.zone = 0;
      return;
   }
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// void commafmt (char *buffer, int bufsz, long number)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *buffer       Pointer to buffer to put string in
//            int bufsz          Size of buffer
//            long number        Number to convert
// Return:    
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Places commas in a long integer number.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void commafmt (char *buffer, int bufsz, long number)
{
int len = 1, posn = 1, sign = 1;
char *ptr = buffer + bufsz - 1;

      Alert (buffer == NULL);
      Alert (bufsz < 3);

      if (2 > bufsz)
      {
ABORT:      *buffer = NUL;
            return;
      }

      *ptr-- = NUL;
      --bufsz;
      if (0L > number)
      {
            sign = -1;
            number = -number;
      }

      for ( ; len <= bufsz; ++len, ++posn)
      {
            *ptr-- = (char)((number % 10L) + '0');
            if (0L == (number /= 10L))
                  break;
            if (0 == (posn % 3))
            {
                  *ptr-- = ',';
                  ++len;
            }
            if (len >= bufsz)
                  goto ABORT;
      }

      if (0 > sign)
      {
            if (len >= bufsz)
                  goto ABORT;
            *ptr-- = '-';
            ++len;
      }

      memmove(buffer, ++ptr, len + 1);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// char *wordwrap (char *string, size_t line_len)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *string       String/buffer to word wrap
//            size_t line_len    Length of each line
// Return:    Pointer to string
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Word wraps a string at line_len columns.
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
static int tab_size = 0;                  /* size to consider tabs as */
static size_t get_word(char *string);     /* returns size of next word*/

void set_tab_size(size_t size)
{
      tab_size = size;
}

char *wordwrap(char *string, size_t line_len)
{
      size_t len,                         /* length of current word */
             current_len = 0;             /* current length of line */
      size_t start_line = 0;              /* index of beginning if line */

      while (0 != (len = get_word(&string[current_len + start_line])))
      {
            if (current_len + len < line_len)
                  current_len += len;
            else
            {
                  string[start_line+current_len] = '\n';
                  start_line += current_len + 1;
                  current_len = 0;
            }
      }
      return string;
}

static size_t get_word(char *string)
{
      register int i = 0, word_len = 0;

      if (!string[0])
            return 0;
      while (isspace(string[i]))
      {
            if ('\t' == string[i])
            {
                  if (0 == tab_size)
                        string[i] = ' ';
                  else  word_len += tab_size-1;
            }
            else if ('\n' == string[i])
                  string[i]=' ';
            word_len++;
            i++;
      }
      while (string[i] && !isspace(string[i++]))
            word_len++;
      return word_len;
}
