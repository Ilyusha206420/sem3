#include "expr.h"
#include "myString.h"
#include <stdio.h>

const char *skipws(const char *p)
{
  while (*p == ' ' || *p == '\t') p++;
  return p;
}

int parse_primary(const char **sp, HashMap *hm);

long parse_number(const char **sp)
{
  const char *p = *sp;
  p = skipws(p);
  int sign = 1;
  if (*p == '+') { p++; }
  else if (*p == '-') { sign = -1; p++; }
  long v = 0;
  while (*p >= '0' && *p <= '9') { v = v * 10 + (*p - '0'); p++; }
  *sp = p;
  return v * sign;
}

long parse_unary(const char **sp, HashMap *hm)
{
  const char *p = skipws(*sp);
  if (*p == '!') { p++; long v = parse_unary(&p, hm); *sp = p; return !v; }
  if (*p == '+') { p++; long v = parse_unary(&p, hm); *sp = p; return v; }
  if (*p == '-') { p++; long v = parse_unary(&p, hm); *sp = p; return -v; }
  return parse_primary(sp, hm);
}

long parse_mul(const char **sp, HashMap *hm)
{
  long v = parse_unary(sp, hm);
  const char *p = skipws(*sp);
  while (*p == '*' || *p == '/' || *p == '%') {
    char op = *p++; long rhs = parse_unary(&p, hm);
    if (op == '*') v = v * rhs;
    else if (op == '/') v = rhs ? v / rhs : 0;
    else v = rhs ? v % rhs : 0;
    p = skipws(p);
  }
  *sp = p; return v;
}

long parse_add(const char **sp, HashMap *hm)
{
  long v = parse_mul(sp, hm);
  const char *p = skipws(*sp);
  while (*p == '+' || *p == '-') {
    char op = *p++; long rhs = parse_mul(&p, hm);
    if (op == '+') v = v + rhs; else v = v - rhs;
    p = skipws(p);
  }
  *sp = p; return v;
}

long parse_rel(const char **sp, HashMap *hm)
{
  long v = parse_add(sp, hm);
  const char *p = skipws(*sp);
  for (;;) {
    if (p[0] == '<' && p[1] == '=') { const char *q = p + 2; long rhs = parse_add(&q, hm); v = v <= rhs; p = skipws(q); }
    else if (p[0] == '>' && p[1] == '=') { const char *q = p + 2; long rhs = parse_add(&q, hm); v = v >= rhs; p = skipws(q); }
    else if (p[0] == '<') { const char *q = p + 1; long rhs = parse_add(&q, hm); v = v < rhs; p = skipws(q); }
    else if (p[0] == '>') { const char *q = p + 1; long rhs = parse_add(&q, hm); v = v > rhs; p = skipws(q); }
    else break;
  }
  *sp = p; return v;
}

long parse_eq(const char **sp, HashMap *hm)
{
  long v = parse_rel(sp, hm);
  const char *p = skipws(*sp);
  for (;;) {
    if (p[0] == '=' && p[1] == '=') { const char *q = p + 2; long rhs = parse_rel(&q, hm); v = v == rhs; p = skipws(q); }
    else if (p[0] == '!' && p[1] == '=') { const char *q = p + 2; long rhs = parse_rel(&q, hm); v = v != rhs; p = skipws(q); }
    else break;
  }
  *sp = p; return v;
}

long parse_and(const char **sp, HashMap *hm)
{
  long v = parse_eq(sp, hm);
  const char *p = skipws(*sp);
  while (p[0] == '&' && p[1] == '&') { const char *q = p + 2; long rhs = parse_eq(&q, hm); v = v && rhs; p = skipws(q); }
  *sp = p; return v;
}

long parse_or(const char **sp, HashMap *hm)
{
  long v = parse_and(sp, hm);
  const char *p = skipws(*sp);
  while (p[0] == '|' && p[1] == '|') { const char *q = p + 2; long rhs = parse_and(&q, hm); v = v || rhs; p = skipws(q); }
  *sp = p; return v;
}

int parse_defined_name(const char **sp, char *outname, size_t outcap)
{
  const char *p = skipws(*sp);
  if (!myStrStr((char*)p, "defined")) return 0;
  p += 7;
  p = skipws(p);
  if (*p == '(') {
    p++; p = skipws(p);
    size_t i = 0;
    while (*p && *p != ')' && i + 1 < outcap) outname[i++] = *p++;
    outname[i] = '\0';
    if (*p == ')') p++;
    *sp = p; return 1;
  }
  size_t i = 0;
  while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_' || (*p >= '0' && *p <= '9')) { if (i + 1 < outcap) outname[i++] = *p; p++; }
  outname[i] = '\0';
  *sp = p; return i > 0;
}

int parse_primary(const char **sp, HashMap *hm)
{
  const char *p = skipws(*sp);
  if (*p == '(') { p++; int v = parse_or(&p, hm); p = skipws(p); if (*p == ')') p++; *sp = p; return v; }
  if (myStrStr((char*)p, "defined")) {
    char name[256] = {0};
    if (parse_defined_name(&p, name, sizeof(name))) {
      int res = hasDefinedName(name);
      if (!res && hm) {
        char valbuf[256] = {0}; char *valptr = valbuf;
        if (HMget(hm, name, &valptr)) res = 1;
      }
      *sp = p; return res;
    }
  }
  if ((*p >= '0' && *p <= '9') || *p == '+' || *p == '-') {
    const char *pp = p;
    long num = parse_number(&pp);
    *sp = pp; return num;
  }
  // identifier: try to read name and look up in HashMap for numeric replacement
  char name[256] = {0};
  size_t i = 0;
  while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_' || (*p >= '0' && *p <= '9')) {
    if (i + 1 < sizeof(name)) name[i++] = *p;
    p++;
  }
  name[i] = '\0';
  if (i > 0 && hm) {
    char valbuf[1024] = {0};
    char *valptr = valbuf;
    if (HMget(hm, name, &valptr)) {
      const char *pp = skipws(valptr);
      if (*pp == '+' || *pp == '-' || (*pp >= '0' && *pp <= '9')) {
        long num = parse_number(&pp);
        *sp = p; return num;
      }
      else {
        const char *q = valptr;
        long sub = parse_or(&q, hm);
        *sp = p; return sub;
      }
    }
  }
  *sp = p; return 0;
}

int evalIfExpr(const char *s, HashMap *hm)
{
  const char *p = s;
  int v = parse_or(&p, hm);
  return v != 0;
}
