// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
// AL_MEM.H: ALIB; Memory allocation tracking                             //
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様 //
#ifndef __AL_MEM_H
#define __AL_MEM_H

typedef struct memtrk_struct
{
   void        *ptr;                /* The pointer that we gave 'em        */
   unsigned    size;                /* Byte of memory allocated            */

   char        file [66];           /* Filename it was alloc'd from        */
   unsigned    line;                /* Line # it was alloc'd from          */
} memtrk_entry;

#ifdef DEBUG
#define MALLOC(sz)      Malloc(sz,__FILE__,__LINE__)
#define FREE(ptr)       Free(ptr,__FILE__,__LINE__)
#define ISVALID(ptr)    IsValid(ptr)
#else
#define MALLOC(sz)      malloc(sz)
#define FREE(ptr)       free(ptr)
#define ISVALID(ptr)    (ptr)
#endif

void *Malloc (unsigned size, char *file, unsigned line);
void  Free   (void *ptr, char *file, unsigned line);
int   IsValid(void *ptr);

int   MemTrkAddPtr (void *ptr, int size, char *file, unsigned line);
void  MemTrkRemovePtr (void *ptr);

int   MemTrkDiagnostics (void);
void  MemTrkDump (void);
void  MemTrkCleanup (void);

#endif
