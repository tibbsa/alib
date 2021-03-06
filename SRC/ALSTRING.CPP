// ���������������������������������������������������������������������� 
// ALSTRING.CPP: ALIB; String functions
// ���������������������������������������������������������������������� 
// Implements various string handling functions, including:
//    strtrim
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALSTRING.CPP<library ALIB>")

// �����������������������������������������������������������������������
// void strtrim (char *str)
// �����������������������������������������������������������������������
// Arguments: char *str          String to trim
// Return:    
// �����������������������������������������������������������������������
// Removes trailing, multiple embedded, and leading spaces.
// �����������������������������������������������������������������������
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

// �����������������������������������������������������������������������
// void kill_filename_trail_slash (char *str)
// �����������������������������������������������������������������������
// Arguments: char *str          String to operate on
// Return:    
// �����������������������������������������������������������������������
// Removes a trailing '\' from a filename/path if one is there.
// �����������������������������������������������������������������������
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

// �����������������������������������������������������������������������
// void add_filename_trail_slash (char *str)
// �����������������������������������������������������������������������
// Arguments: char *str          String to operate on
// Return:    
// �����������������������������������������������������������������������
// Adds a trailing '\' from a filename/path if one is not there.
// �����������������������������������������������������������������������
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

// �����������������������������������������������������������������������
// void parsenetaddr (NETADDR &pa, char *pstr)
// �����������������������������������������������������������������������
// Arguments: NETADDR *pa        Pointer to NETADDR string
//            char *pstr         Pointer to string version
// Return:    
// �����������������������������������������������������������������������
// Parses a net address in pstr
// �����������������������������������������������������������������������
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

// �����������������������������������������������������������������������
// void commafmt (char *buffer, int bufsz, long number)
// �����������������������������������������������������������������������
// Arguments: char *buffer       Pointer to buffer to put string in
//            int bufsz          Size of buffer
//            long number        Number to convert
// Return:    
// �����������������������������������������������������������������������
// Places commas in a long integer number.
// �����������������������������������������������������������������������
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

// �����������������������������������������������������������������������
// char *wordwrap (char *string, size_t line_len)
// �����������������������������������������������������������������������
// Arguments: char *string       String/buffer to word wrap
//            size_t line_len    Length of each line
// Return:    Pointer to string
// �����������������������������������������������������������������������
// Word wraps a string at line_len columns.
// �����������������������������������������������������������������������
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
