#include "preprocessor.h"

#include <stdio.h>
#include "assets/hashMap.h"
#include "assets/stack.h"

int prepairVars(FILE **ifp, char *inpName, FILE **ofp, char *outName, FileStack **fstack, HashMap **hm);

int processFile(char *input, char *output)
{
  FILE *ifp = NULL;
  FILE *ofp = NULL;
  FileStack *fstack = NULL;
  HashMap *hm = NULL;

  int err = prepairVars(&ifp, input, &ofp, output, &fstack, &hm);
  if (err)
    return err;
  
  char ch;
  while (ch = getc(ifp), ch != EOF) {
    fputc(ch, ofp);
  }

  fclose(ifp);
  fclose(ofp);

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

int prepairVars(FILE **ifp, char *inpName, FILE **ofp, char *outName, FileStack **fstack, HashMap **hm)
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
  for (int i = 512; i > 0 && !hm; i /= 2) 
    *hm = HMinit(i);
  if (!hm)
    return closeFilesWithError(*ifp, *ofp, MEMORY_ALLOC_ERROR);

  return 0;
}