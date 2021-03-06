// ���������������������������������������������������������������������� 
// ALMEM.CPP: ALIB; Memory allocation tracking implementation
// ���������������������������������������������������������������������� 
// (This is part of my "utility library"- define STANDALONE and it'll use
// nothing that isn't included here)
// ���������������������������������������������������������������������� 

#include <alib.h>
Set_Alert_File ("ALMEM.CPP<library ALIB>")

// Use MALLOC macro for this!
// �����������������������������������������������������������������������
// void *Malloc (unsigned size, char *file, unsigned line)
// �����������������������������������������������������������������������
// Arguments: unsigned size      Size of block to allocate
//            char *file         (Internal use only - filename of alloc)
//            unsigned line      (Internal use only - line # of alloc)
// Return:    Pointer to block of memory
// �����������������������������������������������������������������������
// Allocates a block of memory (size bytes long), adding the pointer to 
// the memory tracking file.
// �����������������������������������������������������������������������
void *Malloc (unsigned size, char *file, unsigned line)
{
FILE *fp;
void *ptr;

   Alert (size == 0);

   ptr = (void *)malloc (size);
   if (!ptr)
   {
      alib.log.Put ("!Memory allocation error");
      return (NULL);
   }

   if (!MemTrkAddPtr (ptr, size, file, line))
   {
      free (ptr);
      return (NULL);
   }
   
#ifdef DEBUG
   alib.log.Put ("MAlloc: %p (%ub@%u/%s)", ptr, size, line, file);
#endif

   return (ptr);
}

// �����������������������������������������������������������������������
// int MemTrkAddPtr (void *ptr, char *file, unsigned line)
// �����������������������������������������������������������������������
// Arguments: void *ptr          Pointer to add to "memtrack" file.
//            int size           Size of block allocated
//            char *file         (Internal use only - filename of alloc)
//            unsigned line      (Internal use only - line # of alloc)
// Return:    False if there is an error
// �����������������������������������������������������������������������
// Adds ptr to the memory database file...
// �����������������������������������������������������������������������
int MemTrkAddPtr (void *ptr, int size, char *file, unsigned line)
{
#ifdef DEBUG
FILE *fp;
memtrk_entry m;
   Alert (ptr == NULL);
   Alert (file == NULL);
   Alert (*file == '\0');
   Alert (line == 0);

   m.ptr = ptr;
   m.size = size;
   strcpy (m.file, file);
   m.line = line;

   fp = fopen ("MEMTRK.DBG", "ab");

   if (!fp)
   {
      alib.log.Put ("MError updating MEMTRK.DBG file!");
      return (False);
   }
   fwrite (&m, 1, sizeof m, fp);
   fclose (fp);

#endif
   return 1;
}

// �����������������������������������������������������������������������
// void MemTrkRemovePtr (void *ptr);
// �����������������������������������������������������������������������
// Arguments: void *ptr          Pointer to remove from "memtrack" file.
// Return:    
// �����������������������������������������������������������������������
// Removes ptr from the database
// �����������������������������������������������������������������������
void MemTrkRemovePtr (void *ptr)
{
#ifdef DEBUG
FILE *fp;
FILE *op;
memtrk_entry m;

   fp = fopen ("MEMTRK.DBG", "rb");
   if (!fp)
   {
#ifdef DEBUG
      alib.log.Put ("MError reading MEMTRK.DBG file!");
#endif
      return;
   }
   op = fopen ("$$$.mtb", "wb");
   if (!op)
   {
      fclose (fp);
#ifdef DEBUG
      alib.log.Put ("MError writing temporary tracking file!");
#endif
      return;
   }


   void *lastptr=NULL;
   
   while (!feof(fp))
   {
      fread (&m, 1, sizeof m, fp);
      if (lastptr==m.ptr)
         continue;
      
      lastptr=m.ptr;
      if (m.ptr == ptr)
         continue;
      else
         fwrite (&m, 1, sizeof m, op);
   }
   fclose (op);
   fclose (fp);
   unlink ("MEMTRK.DBG");
   rename ("$$$.mtb", "MEMTRK.DBG");
#endif
}

// �����������������������������������������������������������������������
// void Free (void *ptr, char *file, unsigned line)
// �����������������������������������������������������������������������
// Arguments: void *ptr          Pointer to free
// Return:    
// �����������������������������������������������������������������������
// Frees pointer "ptr"
// �����������������������������������������������������������������������
void Free (void *ptr, char *file, unsigned line)
{
   if (!ptr)
      return;

   if (!IsValid(ptr))
   {
      /* Just to keep Carey happy.. */
      alib.log.Put ("!Freeing unallocated pointer %p", ptr);
      alib.log.Put ("!  Line %u, file %s", line, file);
   }


   /* Remove it from the list of pointers. */
#ifdef DEBUG
   alib.log.Put ("MFreeing %p@%u/%s", ptr, line, file);
#endif
   MemTrkRemovePtr (ptr);
   ptr = NULL;
   free (ptr);
}

// �����������������������������������������������������������������������
// int IsValid (void *ptr)
// �����������������������������������������������������������������������
// Arguments: void *ptr          Pointer to check for in memtrack file
// Return:    True if pointer is in memtrack file, otherwise False
// �����������������������������������������������������������������������
// Checks to see if pointer "ptr" is (still) in the memtrack file.  (e.g.
// it hasn't been free'd)
// �����������������������������������������������������������������������
int IsValid (void *ptr)
{
#ifdef DEBUG
FILE *fp;
memtrk_entry m;

   fp = fopen ("MEMTRK.DBG", "rb");
   if (!fp)
   {
      alib.log.Put ("MError reading MEMTRK.DBG file!");
      return (False);
   }
   while (!feof(fp))
   {
      fread (&m, 1, sizeof m, fp);
      if (m.ptr == ptr)
      {
         fclose (fp);
         return 1;
      }
   }

   alib.log.Put ("MPointer %p is NOT valid");
   fclose (fp);
#endif
   return 0;
}

// �����������������������������������������������������������������������
// int MemTrkDiagnostics (void)
// �����������������������������������������������������������������������
// Arguments: 
// Return:    True if the memtrack file is not blank, False if it is
// �����������������������������������������������������������������������
// Sees if everything has been freed
// �����������������������������������������������������������������������
int MemTrkDiagnostics (void)
{
#ifdef DEBUG
FILE *fp;

   fp = fopen ("MEMTRK.DBG", "rb");
   if (!fp)
   {
      alib.log.Put ("MError reading MEMTRK.DBG file!");
      return 0;
   }

   if (filelength(fileno(fp)) != 0L)
   {
      fclose (fp);
      return 1;
   }
   else
   {
      fclose (fp);
      return 0;
   }
#else
   return 1;
#endif
}

// �����������������������������������������������������������������������
// void MemTrkDump (void)
// �����������������������������������������������������������������������
// Arguments: 
// Return:    
// �����������������������������������������������������������������������
// Dumps the contents of the memory tracking file to the screen
// �����������������������������������������������������������������������
void MemTrkDump (void)
{
#ifdef DEBUG
FILE *fp;
memtrk_entry m;

   if (!MemTrkDiagnostics())
      return;

   fp = fopen ("MEMTRK.DBG", "rb");
   if (!fp)
   {
      alib.log.Put ("MError reading MEMTRK.DBG file!");
      return;
   }
   printf ("The following pointers were NOT freed:\n\n");

   alib.log.Put ("MPointers not freed:");

void *lastptr=NULL;

   while (!feof(fp))
   {
      fread (&m, 1, sizeof m, fp);
      if (lastptr==m.ptr)
         continue;
      lastptr=m.ptr;
      printf ("%p\tSize=%-.5u\tFile: %s, Line: %u\n", m.ptr, m.size, m.file, m.line);
      alib.log.Put ("M   %p\tSize=%-.5u\tFile: %s, Line: %u\n", m.ptr, m.size, m.file, m.line);
   }

   fclose (fp);
#endif
}

// �����������������������������������������������������������������������
// void MemTrkCleanup (void)
// �����������������������������������������������������������������������
// Arguments: 
// Return:    
// �����������������������������������������������������������������������
// Deallocates any memory which is currently allocated 
// �����������������������������������������������������������������������
void MemTrkCleanup (void)
{
#ifdef DEBUG
FILE *fp;
memtrk_entry m;

   if (!MemTrkDiagnostics())
      return;

   fp = fopen ("MEMTRK.DBG", "rb");
   if (!fp)
   {
      alib.log.Put ("MError reading MEMTRK.DBG file!");
      return;
   }

void *lastptr=NULL;

   alib.log.Put ("MCLEANUP --------------------------------------");

   while (!feof(fp))
   {
      fread (&m, 1, sizeof m, fp);
      if (lastptr==m.ptr)
         continue;
      lastptr=m.ptr;
      FREE (m.ptr);
   }

   alib.log.Put ("MCLEANUP --------------------------------------");
   fclose (fp);
#endif
}
