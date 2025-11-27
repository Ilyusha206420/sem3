#include "preprocessor.h"

#include <stdio.h>
#include "assets/hashMap.h"
#include "assets/myString.h"
#include "assets/stack.h"

/*
TODO : 
includes, define/undef, if/ifdef/ifndef, macro inserting
* Complete:
* str gluing, comment delete 
*/

int prepairVars(FILE **ifp, char *inpName, FILE **ofp, char *outName, FileStack **fstack, HashMap **hm, myString **ms);
int closeFilesWithError(FILE *fp1, FILE *fp2, int errCode);
int processLine(myString *string, FILE *ofp, HashMap *hm, FileStack *fs);

int processFile(char *input, char *output)
{
  FILE *ifp = NULL;
  FILE *ofp = NULL;
  FileStack *fstack = NULL;
  HashMap *hm = NULL;
  myString *MSbuf = NULL;

  int err = prepairVars(&ifp, input, &ofp, output, &fstack, &hm, &MSbuf);
  if (err)
    return err;
  
  FileStruct *curFile = fstack->top->f;
  err = 0;
  while (curFile) {
    err = MSgetLine(MSbuf, curFile->fp);
    if (err == 1)
      curFile = FStackPop(fstack);
    if (err == -1) 
      return closeFilesWithError(ifp, ofp, MEMORY_ALLOC_ERROR);
    processLine(MSbuf, ofp, hm, fstack); 
  }

  fclose(ifp);
  fclose(ofp);

  return 0;
}

int processLine(myString *string, FILE *ofp, HashMap *hm, FileStack *fs)
{
  char *chptr = string->str;
  while (*chptr) {
    if (*chptr == '#') {
      chptr += 1;
      if (myStrStr(chptr, "include ")) {
        while (*chptr && *chptr != ' ') chptr++;
        while (*chptr && *chptr == ' ') chptr++;
        if (*chptr == '<') {
          
        }

        else if (*chptr == '"') {
          
        }

        else 
          return HEADER_FILE_OPENINNG_ERROR;
      }
        
      else if (myStrStr(chptr, "define ")) {
        while (*chptr++ != ' ');
        while (*chptr++ == ' ');

      }

      else if (myStrStr(chptr, "undef ")) {
        while (*chptr++ != ' ');
        while (*chptr++ == ' ');
      }

      else if (myStrStr(chptr, "if ")) {
        while (*chptr++ != ' ');
        while (*chptr++ == ' ');
      }

      else if (myStrStr(chptr, "ifdef ")) {
        while (*chptr++ != ' ');
        while (*chptr++ == ' ');
      }

      else if (myStrStr(chptr, "ifndef ")) {
        while (*chptr++ != ' ');
        while (*chptr++ == ' ');
      }
    }
    else 
      fputc(*chptr++, ofp);
  }
  fputc('\n', ofp);
  return 0;
}

FILE* openFile(char *fileName, char *mode)
{
  FILE *out = NULL;
  int try = 0;
  while (try++, !out && try < 10000)
    out = fopen(fileName, mode);
  return out;
}

int closeFilesWithError(FILE *fp1, FILE *fp2, int errCode) 
{
  fclose(fp1);
  fclose(fp2);
  return errCode;
}

int prepairVars(FILE **ifp, char *inpName, FILE **ofp, char *outName, FileStack **fstack, HashMap **hm, myString **ms)
{
  *ifp = openFile(inpName, "r");
  if (!ifp)
    return FILE_OPEN_ERROR;
  
  *ofp = openFile(outName, "w");
  if (!ofp) {
    fclose(*ifp);
    return FILE_CREATE_ERROR;
  }
  
  *fstack = FStackInit();
  if (!fstack) 
    return closeFilesWithError(*ifp, *ofp, MEMORY_ALLOC_ERROR);
  
  int error = FStackPush(*fstack, *ifp, inpName, 0);
  if (error) 
    return closeFilesWithError(*ifp, *ofp, STACK_ERROR);
  
  *hm = HMinit(1024);
  for (int i = 512; i > 0 && !*hm; i /= 2) 
    *hm = HMinit(i);
  if (!hm)
    return closeFilesWithError(*ifp, *ofp, MEMORY_ALLOC_ERROR);

  for (unsigned long len = 1024; len > 0 && !*ms; len /= 2)
    *ms = myStringInit(len);
  if (!ms)
    return closeFilesWithError(*ifp, *ofp, MEMORY_ALLOC_ERROR);

  return 0;
}