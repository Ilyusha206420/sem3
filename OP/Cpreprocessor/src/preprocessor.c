#include "preprocessor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assets/hashMap.h"
#include "assets/myString.h"
#include "assets/stack.h"
#include "assets/utils.h"
#include "assets/expr.h"

int *ifActiveStack = NULL;
int *branchTakenStack = NULL;
size_t ifStackTop = 0;
size_t ifStackCap = 0;

char **definedNames = NULL;
size_t defCount = 0;
size_t defCap = 0;

int addDefinedName(const char *name)
{
  if (!name) return -1;
  if (defCount + 1 >= defCap) {
    size_t nc = defCap ? defCap * 2 : 16;
    char **nb = (char**)myAllocMemory(sizeof(char*) * nc);
    if (!nb) return -1;
    for (size_t i = 0; i < defCount; ++i) nb[i] = definedNames[i];
    free(definedNames);
    definedNames = nb;
    defCap = nc;
  }
  size_t len = myStrGetLen((char*)name);
  char *s = (char*)myAllocMemory(len + 1);
  if (!s) return -1;
  myStrCpy((char*)name, &s);
  definedNames[defCount++] = s;
  return 0;
}

void removeDefinedName(const char *name)
{
  if (!name || defCount == 0) return;
  for (size_t i = 0; i < defCount; ++i) {
    if (myStrCmp(definedNames[i], (char*)name)) {
      free(definedNames[i]);
      definedNames[i] = definedNames[defCount - 1];
      defCount--;
      return;
    }
  }
}

int hasDefinedName(const char *name)
{
  if (!name) return 0;
  for (size_t i = 0; i < defCount; ++i) if (myStrCmp(definedNames[i], (char*)name)) return 1;
  return 0;
}

int ensureIfStackCapacity()
{
  if (ifStackTop + 1 < ifStackCap)
    return 0;
  size_t newCap = ifStackCap ? ifStackCap * 2 : 16;
  int *newActive = (int*)myAllocMemory(sizeof(int) * newCap);
  int *newTaken = (int*)myAllocMemory(sizeof(int) * newCap);
  if (!newActive || !newTaken) {
    free(newActive);
    free(newTaken);
    return -1;
  }
  for (size_t i = 0; i < ifStackTop; i++) {
    newActive[i] = ifActiveStack ? ifActiveStack[i] : 0;
    newTaken[i] = branchTakenStack ? branchTakenStack[i] : 0;
  }
  free(ifActiveStack);
  free(branchTakenStack);
  ifActiveStack = newActive;
  branchTakenStack = newTaken;
  ifStackCap = newCap;
  return 0;
}

int currentParentActive()
{
  if (ifStackTop == 0) return 1;
  return ifActiveStack[ifStackTop - 1];
}

int currentActive()
{
  if (ifStackTop == 0) return 1;
  return ifActiveStack[ifStackTop - 1];
}

void pushIfLevel(int condResult)
{
  if (ensureIfStackCapacity() != 0)
    return;
  int parent = currentParentActive();
  int active = parent && condResult;
  ifActiveStack[ifStackTop] = active;
  branchTakenStack[ifStackTop] = (parent && condResult) ? 1 : 0;
  ifStackTop++;
}

void popIfLevel()
{
  if (ifStackTop == 0) return;
  ifStackTop--;
}

void handleElse()
{
  if (ifStackTop == 0) return;
  int parent = (ifStackTop >= 2) ? ifActiveStack[ifStackTop - 2] : 1;
  int taken = branchTakenStack[ifStackTop - 1];
  int newActive = parent && !taken;
  ifActiveStack[ifStackTop - 1] = newActive;
  branchTakenStack[ifStackTop - 1] = taken || newActive;
}

void handleElif(int condResult)
{
  if (ifStackTop == 0) return;
  int parent = (ifStackTop >= 2) ? ifActiveStack[ifStackTop - 2] : 1;
  int taken = branchTakenStack[ifStackTop - 1];
  if (!parent || taken) {
    ifActiveStack[ifStackTop - 1] = 0;
    return;
  }
  int active = condResult ? 1 : 0;
  ifActiveStack[ifStackTop - 1] = active;
  if (active) branchTakenStack[ifStackTop - 1] = 1;
}

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
  
  FileStruct *curFile = NULL;
  err = 0;
  while ((fstack->top) && (curFile = fstack->top->f)) {
    err = MSgetLine(MSbuf, curFile->fp);
    if (err == -1)
      return closeFilesWithError(ifp, ofp, MEMORY_ALLOC_ERROR);
    processLine(MSbuf, ofp, hm, fstack);
    if (err == 1)
      FStackPop(fstack);
  }

  fclose(ifp);
  fclose(ofp);

  return 0;
}

FILE* openFile(char *fileName, char *mode);

int processLine(myString *string, FILE *ofp, HashMap *hm, FileStack *fs)
{
  if (!string || !string->str)
    return 0;

  char *s = string->str;
  size_t i = 0;
  /* skip leading whitespace */
  while (s[i] == ' ' || s[i] == '\t') i++;

  if (s[i] == '#') {
    i++;
    while (s[i] == ' ' || s[i] == '\t') i++;
    /* read directive */
    char dir[64] = {0};
    size_t di = 0;
    while (s[i] && s[i] != ' ' && s[i] != '\t') {
      if (di + 1 < sizeof(dir)) dir[di++] = s[i];
      i++;
    }
    dir[di] = '\0';

    if (myStrCmp(dir, "include")) {
      if (!currentActive()) return 0;
      while (s[i] == ' ' || s[i] == '\t') i++;
      if (s[i] == '"') {
        i++;
        char incname[1024] = {0};
        size_t ii = 0;
        while (s[i] && s[i] != '"' && ii + 1 < sizeof(incname))
          incname[ii++] = s[i++];
        incname[ii] = '\0';

        /* build path relative to current file */
        char filepath[2048] = {0};
        char *curfn = NULL;
        if (fs && fs->top && fs->top->f && fs->top->f->filename)
          curfn = fs->top->f->filename;

        if (curfn) {
          char *lastSlash = strrchr(curfn, '/');
          char *lastBack = strrchr(curfn, '\\');
          char *sep = lastSlash > lastBack ? lastSlash : lastBack;
          if (sep) {
            size_t dirlen = sep - curfn + 1;
            if (dirlen + myStrGetLen(incname) + 1 < sizeof(filepath)) {
              strncpy(filepath, curfn, dirlen);
              filepath[dirlen] = '\0';
              strncat(filepath, incname, sizeof(filepath) - myStrGetLen(filepath) - 1);
            }
          }
        }
        if (filepath[0] == '\0')
          strncpy(filepath, incname, sizeof(filepath)-1);

        FILE *nf = openFile(filepath, "r");
        if (nf) {
          FStackPush(fs, nf, filepath, 0);
        }
      }
    }
    else if (myStrCmp(dir, "ifdef")) {
      while (s[i] == ' ' || s[i] == '\t') i++;
      char key[256] = {0}; size_t ki = 0;
      while (s[i] && s[i] != ' ' && s[i] != '\t') { if (ki + 1 < sizeof(key)) key[ki++] = s[i]; i++; }
      key[ki] = '\0';
      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) { key[--ki] = '\0'; }
      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) { key[--ki] = '\0'; }
      int cond = 0;
      if (myStrGetLen(key) > 0) {
        /* check our defined list first, then hashmap */
        if (hasDefinedName(key)) cond = 1;
        else if (hm) { char *tmp = NULL; cond = HMget(hm, key, &tmp); }
      }
      pushIfLevel(cond);
    }
    else if (myStrCmp(dir, "ifndef")) {
      while (s[i] == ' ' || s[i] == '\t') i++;
      char key[256] = {0}; size_t ki = 0;
      while (s[i] && s[i] != ' ' && s[i] != '\t') { if (ki + 1 < sizeof(key)) key[ki++] = s[i]; i++; }
      key[ki] = '\0';
      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) { key[--ki] = '\0'; }
      int cond = 1;
      if (myStrGetLen(key) > 0) {
        if (hasDefinedName(key)) cond = 0;
        else if (hm) { char *tmp = NULL; cond = !HMget(hm, key, &tmp); }
      }
      pushIfLevel(cond);
    }
    else if (myStrCmp(dir, "if")) {
      while (s[i] == ' ' || s[i] == '\t') i++;
      int cond = evalIfExpr(s + i, hm);
      pushIfLevel(cond);
    }
    else if (myStrCmp(dir, "else")) {
      handleElse();
    }
    else if (myStrCmp(dir, "elif")) {
      while (s[i] == ' ' || s[i] == '\t') i++;
      int cond = evalIfExpr(s + i, hm);
      handleElif(cond);
    }
    else if (myStrCmp(dir, "endif")) {
      popIfLevel();
    }
    else if (myStrCmp(dir, "define")) {
      if (!currentActive()) return 0;
      while (s[i] == ' ' || s[i] == '\t') i++;
      char key[256] = {0};
      size_t ki = 0;
      while (s[i] && s[i] != ' ' && s[i] != '\t') {
        if (ki + 1 < sizeof(key)) key[ki++] = s[i];
        i++;
      }
      key[ki] = '\0';
      while (s[i] == ' ' || s[i] == '\t') i++;
      char val[1024] = {0};
      size_t vi = 0;
      while (s[i] && vi + 1 < sizeof(val)) val[vi++] = s[i++];
      val[vi] = '\0';
      if (myStrGetLen(key) > 0) {
        HMadd(hm, key, val);
        addDefinedName(key);
      }
    }
    else if (myStrCmp(dir, "undef")) {
      if (!currentActive()) return 0;
      while (s[i] == ' ' || s[i] == '\t') i++;
      char key[256] = {0};
      size_t ki = 0;
      while (s[i] && s[i] != ' ' && s[i] != '\t') {
        if (ki + 1 < sizeof(key)) key[ki++] = s[i];
        i++;
      }
      key[ki] = '\0';
      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) { key[--ki] = '\0'; }
      if (myStrGetLen(key) > 0) {
        HMdelete(hm, key);
        removeDefinedName(key);
      }
    }
    return 0;
  }

  if (!currentActive())
    return 0;

  char token[512];
  char replbuf[2048];
  size_t idx = 0;
  int hadContent = 0;
  while (s[idx]) {
    if ((s[idx] >= 'a' && s[idx] <= 'z') || (s[idx] >= 'A' && s[idx] <= 'Z') || s[idx] == '_') {
      size_t ti = 0;
      size_t j = idx;
      while ((s[j] >= 'a' && s[j] <= 'z') || (s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= '0' && s[j] <= '9') || s[j] == '_') {
        if (ti + 1 < sizeof(token)) token[ti++] = s[j];
        j++;
      }
      token[ti] = '\0';
      char *p = replbuf;
      int got = HMget(hm, token, &p);
      if (got) {
        fputs(replbuf, ofp);
        hadContent = 1;
      }
      else {
        fwrite(token, 1, myStrGetLen(token), ofp);
        hadContent = 1;
      }
      idx = j;
    }
    else {
      fputc(s[idx], ofp);
      if (s[idx] != ' ' && s[idx] != '\t') hadContent = 1;
      idx++;
    }
  }
  if (hadContent) fputc('\n', ofp);
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
  if (!*ifp)
    return FILE_OPEN_ERROR;

  *ofp = openFile(outName, "w");
  if (!*ofp) {
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
  if (!*hm)
    return closeFilesWithError(*ifp, *ofp, MEMORY_ALLOC_ERROR);

  for (unsigned long len = 1024; len > 0 && !*ms; len /= 2)
    *ms = myStringInit(len);
  if (!*ms)
    return closeFilesWithError(*ifp, *ofp, MEMORY_ALLOC_ERROR);

  return 0;
}