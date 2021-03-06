// ���������������������������������������������������������������������� 
// ALFILE.CPP: ALIB; File management
// ���������������������������������������������������������������������� 
// File/directory management
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALFILE.CPP<library ALIB>")

// �����������������������������������������������������������������������
// int fexist (char *file)
// �����������������������������������������������������������������������
// Arguments: char *File         Filename to check
// Return:    0 if file does not exist, 1 if it does
// �����������������������������������������������������������������������
// Checks to see if file "file" exists.
// �����������������������������������������������������������������������
int fexist (char *file)
{
struct stat sb;

   Alert (file == NULL);
   Alert (*file == NUL);

   if (stat(file, &sb) == -1)
      return 0;
   return 1;
}

// �����������������������������������������������������������������������
// int pathexist (char *path)
// �����������������������������������������������������������������������
// Arguments: char *path         Pathname to check
// Return:    0 if path does not exist, 1 if it does
// �����������������������������������������������������������������������
// Checks to see if the path (path) exists.
// �����������������������������������������������������������������������
int pathexist (char *path)
{
char p [120];

   Alert (path == NULL);
   Alert (*path == NUL);

   strcpy (p, path);
   kill_filename_trail_slash (p);
   strcat (p, "\\NUL");
   return (fexist(p));
}

// �����������������������������������������������������������������������
// int makepath (char *path)
// �����������������������������������������������������������������������
// Arguments: char *path         Pathname to create
// Return:    0 if path wasn't created, 1 if it was
// �����������������������������������������������������������������������
// Creates path (and all directories above it).
// �����������������������������������������������������������������������
int makepath (char *path)
{
char mine[120];
char subpath [120]="", *p;

   Alert (path == NULL);
   Alert (*path == NUL);

   if (*(path + 1) = ':')
      return (False);

   strcpy (mine, path);
   kill_filename_trail_slash (mine);
   if (pathexist (mine))
      return (True);

   while ((mkdir (mine)) != 0)
   {
      p = strrchr (mine, '\\');
      if (!p)
         return 0;

      if (*(p + 1) = ':')
         return (False);

      strncat (subpath, mine, (word)(p - mine));
      
      if (!makepath (subpath))
         break;
   }

   return (mkdir (mine));
}
