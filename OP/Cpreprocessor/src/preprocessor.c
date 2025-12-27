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

int prepairVars(FILE **ifp, char *inpName, FILE **ofp, char *outName, FileStack **fstack, HashMap **hm, myString **ms);
int closeFilesWithError(FILE *fp1, FILE *fp2, int errCode);
int processLine(myString *string, FILE *ofp, HashMap *hm, FileStack *fs);

int isIdentChar(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c >= '0' && c <= '9');
}

/* Expand only object-like macros in `src` and write result to `ofp`. Function-like macros are not invoked here. */
void expandStringAndWrite(char *src, HashMap *hm, FILE *ofp)
{
  size_t idx = 0;
  while (src[idx]) {
    if ((src[idx] >= 'a' && src[idx] <= 'z') || (src[idx] >= 'A' && src[idx] <= 'Z') || src[idx] == '_') {
      char token[512] = {0};
      size_t ti = 0;
      size_t j = idx;
      while (isIdentChar(src[j])) {
        if (ti + 1 < sizeof(token)) token[ti++] = src[j];
        j++;
      }
      token[ti] = '\0';

      char replbuf[4096]; char *p = replbuf;
      int got = HMget(hm, token, &p);
      if (got) {
        if (myStrGetLen(replbuf) >= 6 && replbuf[0] == '<' && replbuf[1]=='F' && replbuf[2]=='U' && replbuf[3]=='N' && replbuf[4]=='C' && replbuf[5]=='>') {
          // function-like macro stored; do not invoke here, just write token
          fwrite(token, 1, myStrGetLen(token), ofp);
        } else {
          fputs(replbuf, ofp);
        }
      } else {
        fwrite(token, 1, myStrGetLen(token), ofp);
      }
      idx = j;
    } else {
      fputc(src[idx], ofp);
      idx++;
    }
  }
}

// основная функция обработки файла
int processFile(char *input, char *output)
{
  FILE *ifp = NULL; // указатель на входной файл
  FILE *ofp = NULL; // указатель на результрующий файл
  FileStack *fstack = NULL; // указатель на структуру, хранящую стэк используемых файлов
  HashMap *hm = NULL; // указатель на структуру хэш таблицы
  myString *MSbuf = NULL; // указатель на структуру, хранящую строку, прочтенную из файла

  int err = prepairVars(&ifp, input, &ofp, output, &fstack, &hm, &MSbuf); // инициализация переменных
  if (err) 
    return err; // если возникла ошибка, возврат кода ошибки
  
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

  return closeFilesWithError(ifp, ofp, 0);
}

int currentActive();
FILE* openFile(char *fileName, char *mode);
void pushIfLevel(int condResult);
void handleElse();
void handleElif(int condResult);
void popIfLevel();

// функция обработки строки
int processLine(myString *string, FILE *ofp, HashMap *hm, FileStack *fs)
{
  if (!string || !string->str)
    return 0; // если не передана строка, возвращает 0

  char *s = string->str;
  size_t i = 0;
  while (s[i] == ' ' || s[i] == '\t') // пропуск пробелов
    i++;

  if (s[i] == '#') {
    i++;
    while (s[i] == ' ' || s[i] == '\t') 
      i++;
    char dir[64] = {0};
    size_t di = 0;
    while (s[i] && s[i] != ' ' && s[i] != '\t') {
      if (di + 1 < sizeof(dir)) 
        dir[di++] = s[i];
      i++;
    }
    dir[di] = '\0';

    if (myStrCmp(dir, "include")) {
      if (!currentActive()) 
        return 0;
      while (s[i] == ' ' || s[i] == '\t') 
        i++;
      if (s[i] == '"') {
        i++;
        char incname[1024] = {0};
        size_t ii = 0;
        while (s[i] && s[i] != '"' && ii + 1 < sizeof(incname))
          incname[ii++] = s[i++];
        incname[ii] = '\0';

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
      while (s[i] == ' ' || s[i] == '\t') 
        i++;
      char key[256] = {0}; 
      size_t ki = 0;
      while (s[i] && s[i] != ' ' && s[i] != '\t') { 
        if (ki + 1 < sizeof(key)) 
          key[ki++] = s[i]; 
        i++; 
      }

      key[ki] = '\0';
      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) 
        key[--ki] = '\0'; 

      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) 
        key[--ki] = '\0'; 

      int cond = 0;
      if (myStrGetLen(key) > 0)
          cond = HMget(hm, key, NULL); 

      pushIfLevel(cond);
    }
    else if (myStrCmp(dir, "ifndef")) {
      while (s[i] == ' ' || s[i] == '\t') 
        i++;

      char key[256] = {0}; 
      size_t ki = 0;
      while (s[i] && s[i] != ' ' && s[i] != '\t') { 
        if (ki + 1 < sizeof(key)) 
          key[ki++] = s[i]; 
        i++; 
      }

      key[ki] = '\0';
      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) 
        key[--ki] = '\0'; 

      int cond = 1;
      if (myStrGetLen(key) > 0)
        cond = !HMget(hm, key, NULL); 

      pushIfLevel(cond);
    }

    else if (myStrCmp(dir, "if")) {
      while (s[i] == ' ' || s[i] == '\t') 
        i++;
      int cond = evalIfExpr(s + i, hm);
      pushIfLevel(cond);
    }
    else if (myStrCmp(dir, "else")) {
      handleElse();
    }
    else if (myStrCmp(dir, "elif")) {
      while (s[i] == ' ' || s[i] == '\t') 
        i++;
      int cond = evalIfExpr(s + i, hm);
      handleElif(cond);
    }
    else if (myStrCmp(dir, "endif")) {
      popIfLevel();
    }
    else if (myStrCmp(dir, "define")) {
      if (!currentActive()) 
        return 0;
      while (s[i] == ' ' || s[i] == '\t') 
        i++;
      char key[256] = {0};
      size_t ki = 0;
      size_t j = i; // remember position after name
      while (s[j] && s[j] != ' ' && s[j] != '\t' && s[j] != '(') {
        if (ki + 1 < sizeof(key)) 
          key[ki++] = s[j];
        j++;
      }
      key[ki] = '\0';

      // detect function-like macro if '(' follows name immediately
      int isFunc = 0;
      size_t paramsStart = 0;
      if (s[j] == '(') {
        isFunc = 1;
        paramsStart = j + 1;
      }

      if (isFunc) {
        // parse parameter list into a buffer where params are separated by 0x1F
        char params[512];
        size_t pi = 0;
        size_t ppos = paramsStart;
        while (s[ppos] && s[ppos] != ')') {
          if (s[ppos] == ' ' || s[ppos] == '\t') { ppos++; continue; }
          if (s[ppos] == ',') { if (pi + 1 < sizeof(params)) params[pi++] = '\x1F'; ppos++; continue; }
          // read param name
          char pname[64]; size_t pni = 0;
          while (s[ppos] && s[ppos] != ',' && s[ppos] != ')' && s[ppos] != ' ' && s[ppos] != '\t') {
            if (pni + 1 < sizeof(pname)) pname[pni++] = s[ppos];
            ppos++;
          }
          pname[pni] = '\0';
          for (size_t k = 0; k < pni && pi + 1 < sizeof(params); k++) params[pi++] = pname[k];
        }
        params[pi] = '\0';

        // advance j past the closing ')'
        while (s[j] && s[j] != ')') j++;
        if (s[j] == ')') j++;

        // skip whitespace to body
        while (s[j] == ' ' || s[j] == '\t') j++;

        char val[2048] = {0};
        size_t vi = 0;
        while (s[j] && vi + 1 < sizeof(val)) val[vi++] = s[j++];
        val[vi] = '\0';

        // encode as: <FUNC>\x1Fparam1\x1Fparam2\x1E<body>
        char enc[4096]; size_t ei = 0;
        const char *marker = "<FUNC>";
        for (size_t m = 0; marker[m]; m++) enc[ei++] = marker[m];
        enc[ei++] = '\x1F';
        for (size_t m = 0; params[m]; m++) enc[ei++] = params[m];
        enc[ei++] = '\x1E';
        for (size_t m = 0; val[m]; m++) enc[ei++] = val[m];
        enc[ei] = '\0';

        if (myStrGetLen(key) > 0)
          HMadd(hm, key, enc);
      } else {
        // object-like macro
        i = j;
        while (s[i] == ' ' || s[i] == '\t') i++;
        char val[1024] = {0};
        size_t vi = 0;
        while (s[i] && vi + 1 < sizeof(val)) val[vi++] = s[i++];
        val[vi] = '\0';
        if (myStrGetLen(key) > 0)
          HMadd(hm, key, val);
      }
    }
    else if (myStrCmp(dir, "undef")) {
      if (!currentActive()) 
        return 0;

      while (s[i] == ' ' || s[i] == '\t') 
        i++;
      char key[256] = {0};
      size_t ki = 0;
      while (s[i] && s[i] != ' ' && s[i] != '\t') {
        if (ki + 1 < sizeof(key)) 
          key[ki++] = s[i];
        i++;
      }

      key[ki] = '\0';
      while (ki > 0 && (key[ki-1] == '\r' || key[ki-1] == ' ' || key[ki-1] == '\t')) 
        key[--ki] = '\0';
      if (myStrGetLen(key) > 0) 
        HMdelete(hm, key);
    }
    return 0;
  }

  if (!currentActive())
    return 0;

  char token[512];
  char replbuf[4096];
  size_t idx = 0;
  int hadContent = 0;
  while (s[idx]) {
    if ((s[idx] >= 'a' && s[idx] <= 'z') || (s[idx] >= 'A' && s[idx] <= 'Z') || s[idx] == '_') {
      size_t ti = 0;
      size_t j = idx;
      while (isIdentChar(s[j])) {
        if (ti + 1 < sizeof(token)) 
          token[ti++] = s[j];
        j++;
      }
      token[ti] = '\0';
      char *p = replbuf;
      int got = HMget(hm, token, &p);
      if (got) {
        if (myStrGetLen(replbuf) >= 6 && strncmp(replbuf, "<FUNC>", 6) == 0) {
          // function-like macro stored as: <FUNC>\x1Fparam1\x1Fparam2\x1E<body>
          // invoke only if '(' immediately follows the identifier
          if (s[j] == '(') {
            // parse call arguments (handle nested parentheses)
            size_t callPos = j + 1;
            int depth = 1;
            char argsText[2048]; size_t ai = 0;
            while (s[callPos] && depth > 0) {
              char c = s[callPos];
              if (c == '(') depth++;
              else if (c == ')') depth--;
              if (depth == 0) break;
              if (ai + 1 < sizeof(argsText)) argsText[ai++] = c;
              callPos++;
            }
            argsText[ai] = '\0';

            // split argsText by top-level commas
            char argVals[64][1024];
            int argCount = 0;
            int d2 = 0;
            size_t pos = 0;
            char curArg[1024]; size_t cai = 0;
            while (argsText[pos]) {
              char c = argsText[pos];
              if (c == '(') { d2++; if (cai + 1 < sizeof(curArg)) curArg[cai++] = c; }
              else if (c == ')') { d2--; if (cai + 1 < sizeof(curArg)) curArg[cai++] = c; }
              else if (c == ',' && d2 == 0) {
                curArg[cai] = '\0';
                // trim leading and trailing spaces/tabs
                char *start = curArg; while (*start == ' ' || *start == '\t') start++;
                char *end = start + strlen(start);
                while (end > start && (*(end-1) == ' ' || *(end-1) == '\t')) end--;
                *end = '\0';
                if (argCount < 64) strcpy(argVals[argCount++], start);
                cai = 0; memset(curArg, 0, sizeof(curArg));
              }
              else { if (cai + 1 < sizeof(curArg)) curArg[cai++] = c; }
              pos++;
            }
            if (cai > 0) {
              curArg[cai] = '\0';
              // trim leading/trailing whitespace for last arg
              char *start = curArg; while (*start == ' ' || *start == '\t') start++;
              char *end = start + strlen(start);
              while (end > start && (*(end-1) == ' ' || *(end-1) == '\t')) end--;
              *end = '\0';
              if (argCount < 64) strcpy(argVals[argCount++], start);
            }

            // decode params and body from replbuf
            char *paramsStartPtr = replbuf + 6; // after "<FUNC>"
            if (*paramsStartPtr == '\x1F') paramsStartPtr++;
            char *sep = strchr(paramsStartPtr, '\x1E');
            char paramsBuf[512]; paramsBuf[0] = '\0';
            if (sep) {
              size_t plen = sep - paramsStartPtr;
              if (plen >= sizeof(paramsBuf)) plen = sizeof(paramsBuf) - 1;
              memcpy(paramsBuf, paramsStartPtr, plen);
              paramsBuf[plen] = '\0';
            }

            char paramNames[64][64]; int paramCount = 0;
            size_t pk = 0; char tmp[64]; size_t tki = 0;
            while (paramsBuf[pk]) {
              char c = paramsBuf[pk];
              if (c == '\x1F') { tmp[tki] = '\0'; if (paramCount < 64) strcpy(paramNames[paramCount++], tmp); tki = 0; }
              else { if (tki + 1 < sizeof(tmp)) tmp[tki++] = c; }
              pk++;
            }
            if (tki > 0) { tmp[tki] = '\0'; if (paramCount < 64) strcpy(paramNames[paramCount++], tmp); }

            char *body = sep ? sep + 1 : (replbuf + 6);

            // substitute parameters in body
            char res[8192]; size_t ri = 0;
            size_t bpos = 0;
            while (body[bpos]) {
              if ((body[bpos] >= 'a' && body[bpos] <= 'z') || (body[bpos] >= 'A' && body[bpos] <= 'Z') || body[bpos] == '_') {
                char id[256]; size_t idi = 0; size_t bj = bpos;
                while (isIdentChar(body[bj])) { if (idi + 1 < sizeof(id)) id[idi++] = body[bj]; bj++; }
                id[idi] = '\0';
                int replaced = 0;
                for (int pn = 0; pn < paramCount; pn++) {
                  if (myStrCmp(id, paramNames[pn])) {
                    size_t k = 0; while (argVals[pn][k] && ri + 1 < sizeof(res)) res[ri++] = argVals[pn][k++];
                    replaced = 1; break;
                  }
                }
                if (!replaced) { size_t k = 0; while (id[k] && ri + 1 < sizeof(res)) res[ri++] = id[k++]; }
                bpos = bj;
              } else {
                if (ri + 1 < sizeof(res)) res[ri++] = body[bpos];
                bpos++;
              }
            }
            res[ri] = '\0';

            // expand object-like macros inside substituted result and write
            expandStringAndWrite(res, hm, ofp);
            hadContent = 1;

            // advance idx past the closing ')'
            idx = callPos + 1;
            continue;
          } else {
            // not a call; just write token name
            fwrite(token, 1, myStrGetLen(token), ofp);
            hadContent = 1;
          }
        } else {
          // object-like macro: write replacement
          fputs(replbuf, ofp);
          hadContent = 1;
        }
      } else {
        fwrite(token, 1, myStrGetLen(token), ofp);
        hadContent = 1;
      }
      idx = j;
    }
    else {
      fputc(s[idx], ofp);
      if (s[idx] != ' ' && s[idx] != '\t') 
        hadContent = 1;
      idx++;
    }
  }
  if (hadContent) 
    fputc('\n', ofp);
  
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
  if (ifStackTop == 0) 
    return 1;
  return ifActiveStack[ifStackTop - 1];
}

int currentActive()
{
  if (ifStackTop == 0) 
    return 1;
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
  if (ifStackTop == 0) 
    return;
  ifStackTop--;
}

void handleElse()
{
  if (ifStackTop == 0) 
    return;
  int parent = (ifStackTop >= 2) ? ifActiveStack[ifStackTop - 2] : 1;
  int taken = branchTakenStack[ifStackTop - 1];
  int newActive = parent && !taken;
  ifActiveStack[ifStackTop - 1] = newActive;
  branchTakenStack[ifStackTop - 1] = taken || newActive;
}

void handleElif(int condResult)
{
  if (ifStackTop == 0) 
    return;
  
  int parent = (ifStackTop >= 2) ? ifActiveStack[ifStackTop - 2] : 1;
  int taken = branchTakenStack[ifStackTop - 1];
  if (!parent || taken) {
    ifActiveStack[ifStackTop - 1] = 0;
    return;
  }
  int active = condResult ? 1 : 0;
  ifActiveStack[ifStackTop - 1] = active;
  if (active) 
    branchTakenStack[ifStackTop - 1] = 1;
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
  *ifp = openFile(inpName, "rt");
  if (!*ifp)
    return FILE_OPEN_ERROR;

  *ofp = openFile(outName, "wt");
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
