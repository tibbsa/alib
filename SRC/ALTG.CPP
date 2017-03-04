// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 
// ALTG.CPP: ALIB; Telegard Support
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ 
// Implements bound support for Telegard interfacing
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 

#include <alib.h>
Set_Alert_File ("ALTG.CPP<library ALIB>")

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// int tgReadConfig (const char *path, configrec *c);
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Arguments: char *path         Path to main telegard directory
//            configrec *c       Pointer to "configrec" structure.
// Return:    False if an error occurs
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Reads path\\CONFIG.TG into "c"
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int tgReadConfig (const char *path, configrec *c)
{
FILE *fp;
char fullpath[120];

   Alert (path == NULL);
   Alert (*path == NUL);
   Alert (c == NULL);

   sprintf (fullpath, "%s\\CONFIG.TG", path);
#ifdef DEBUG
   alib.log.Put ("MTELEGARD: Reading configuration from %s", fullpath);
#endif

   fp = _fsopen (fullpath, "rb", SH_DENYWR);
   if (!fp)
   {
      alib.log.Put ("!Error opening Telegard configuration \"%s\"", fullpath);
      return (False);
   }

   fread (c, 1, sizeof (configrec), fp);
   fclose (fp);

#ifdef DEBUG
   alib.log.Put ("MTELEGARD: Version #=%-4.4X", c->versionid);
#endif

   /* Convert Pascal strings to C/C++ */
#define p2c(x) P2Cconvert(x)

   p2c (c->datapath);
   p2c (c->textpath);
   p2c (c->langpath);
   p2c (c->menupath);
   p2c (c->logspath);
   p2c (c->msgpath);
   p2c (c->semaphore);
   p2c (c->attchpath);
   p2c (c->swappath);
   p2c (c->arcpath);
   p2c (c->protpath);
   p2c (c->filepath);
   p2c (c->scriptpath);
   p2c (c->bbsname);
   p2c (c->bbsphone);
   p2c (c->bbslocation);
   p2c (c->sysopname);
   p2c (c->bulletprefix);
   p2c (c->sysoppw);
   p2c (c->externalchat);
   p2c (c->fseforcedcmd);
   p2c (c->fsecmd);
   p2c (c->logonmenu);
   p2c (c->watcharea);
   p2c (c->inquiryarea);
   p2c (c->newapp);
   p2c (c->language);
   p2c (c->crosspost);
   p2c (c->batchfileext);
   p2c (c->os2cmdfileext);
   p2c (c->ccmail);
   p2c (c->creq);
   p2c (c->fileattach);
   p2c (c->normpubpost);
   p2c (c->normprivpost);
   p2c (c->normnetpost);
   p2c (c->nopostratio);
   p2c (c->anonpubread);
   p2c (c->anonprivread);
   p2c (c->anonpubpost);
   p2c (c->anonprivpost);
   p2c (c->fileattachnet);
   p2c (c->writescanlog);
   p2c (c->sop);
   p2c (c->csop);
   p2c (c->spw);
   p2c (c->qwkpktname);
   p2c (c->qwkwelcomefile);
   p2c (c->qwknewsfile);
   p2c (c->qwkgoodbyefile);
   p2c (c->qwklocaldlpath);
   p2c (c->qwklocalulpath);
   p2c (c->qwkarc);
   p2c (c->qwknewsgen);
   p2c (c->qwkfilesgen);
   p2c (c->seeunavail);
   p2c (c->ularea);
   p2c (c->filepwacs);
   p2c (c->sysopularea);
   p2c (c->seeunval);
   p2c (c->dlunval);
   p2c (c->ulvalreq);
   p2c (c->virusscanner);
   p2c (c->testpath);
   p2c (c->dlunavail);
#undef p2c

   return (True);
}


