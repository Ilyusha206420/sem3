
#include "expr.h"

#include "hashMap.h"
#include "myString.h"
#include <stdio.h>

/*
  Разбор и вычисление арифметико-логических выражений,
  используемых в директивах `#if` и подобных.
  Реализован рекурсивный нисходящий парсер с приоритетами.
*/

// Пропустить пробельные символы и табуляцию
char* skipws(char *p)
{
  while (*p == ' ' || *p == '\t') p++;
  return p;
}

int parse_primary(char **sp, HashMap *hm);

// функция парсинга чисел
long parse_number(char **sp)
{
  char *p = *sp;
  p = skipws(p);
  int sign = 1;
  if (*p == '+')  
    p++;

  else if (*p == '-') {
    sign = -1; 
    p++;
  }

  long v = 0;
  while (*p >= '0' && *p <= '9') { 
    v = v * 10 + (*p - '0'); 
    p++; 
  }
  *sp = p;
  return v * sign;
}

// рукурсивная функция обработки унарных операторов
long parse_unary(char **sp, HashMap *hm)
{
  char *p = skipws(*sp);
  if (*p == '!') { 
    p++; 
    long v = parse_unary(&p, hm); // рекурсивно обрабатываем унарные операторы после '!'
    *sp = p; 
    return !v; 
  }
  
  if (*p == '+') { 
    p++; 
    long v = parse_unary(&p, hm); // рекурсивно обрабатываем унарные операторы после '+'
    *sp = p; 
    return v; 
  }

  if (*p == '-') { 
    p++; 
    long v = parse_unary(&p, hm); // рекурсивно обрабатываем унарные операторы после '-'
    *sp = p; 
    return -v; 
  }
  
  // если не унарный оператор - обрабатываем как первичное выражение
  return parse_primary(sp, hm);
}

// функция обработки умножения и деления
long parse_mul(char **sp, HashMap *hm)
{
  long v = parse_unary(sp, hm); //v - первое число
  char *p = skipws(*sp); // пропускаем пробелы
  while (*p == '*' || *p == '/' || *p == '%') { 
    char op = *p++; // op - оператор
    long rhs = parse_unary(&p, hm); // rhs - число, либо выражение после оператора
    if (op == '*') 
      v = v * rhs;
    
    else if (op == '/') 
      v = rhs ? v / rhs : 0;
    
    else 
      v = rhs ? v % rhs : 0;
    
    p = skipws(p);
  }
  *sp = p; 
  return v;
}

// функция обработки сложения и вычитания
long parse_add(char **sp, HashMap *hm)
{
  long v = parse_mul(sp, hm); // v - первое число
  char *p = skipws(*sp); 
  while (*p == '+' || *p == '-') {
    char op = *p++; // op - оператор
    long rhs = parse_mul(&p, hm); //rhs - число, либо выражение после оператора
    if (op == '+') 
      v = v + rhs; 
    
    else 
      v = v - rhs;
    p = skipws(p);
  }
  *sp = p; return v;
}

// функция обработки операторов сравнения
long parse_rel(char **sp, HashMap *hm)
{
  long v = parse_add(sp, hm); // v - первое число
  char *p = skipws(*sp);
  for (;;) {
    if (p[0] == '<' && p[1] == '=') { 
      char *q = p + 2; 
      long rhs = parse_add(&q, hm); // число, либо выражение после оператора
      v = v <= rhs; 
      p = skipws(q); 
    }
    else if (p[0] == '>' && p[1] == '=') { 
      char *q = p + 2; 
      long rhs = parse_add(&q, hm); // число, либо выражение после оператора
      v = v >= rhs; 
      p = skipws(q); 
    }
    else if (p[0] == '<') { 
      char *q = p + 1; 
      long rhs = parse_add(&q, hm); // число, либо выражение после оператора
      v = v < rhs; 
      p = skipws(q); 
    }
    else if (p[0] == '>') { 
      char *q = p + 1; 
      long rhs = parse_add(&q, hm); // число, либо выражение после оператора
      v = v > rhs; 
      p = skipws(q); 
    }
    else break;
  }
  *sp = p; 
  return v;
}

// функция обработки операторов равенства и неравенства
long parse_eq(char **sp, HashMap *hm)
{
  long v = parse_rel(sp, hm);
  char *p = skipws(*sp);
  for (;;) {
    if (p[0] == '=' && p[1] == '=') { 
      char *q = p + 2; 
      long rhs = parse_rel(&q, hm); 
      v = v == rhs; 
      p = skipws(q); 
    }
    else if (p[0] == '!' && p[1] == '=') { 
      char *q = p + 2; 
      long rhs = parse_rel(&q, hm); 
      v = v != rhs; 
      p = skipws(q); 
    }
    else break;
  }
  *sp = p; 
  return v;
}

// функция обработки оператора логического И
long parse_and(char **sp, HashMap *hm)
{
  long v = parse_eq(sp, hm);
  char *p = skipws(*sp);
  while (p[0] == '&' && p[1] == '&') { 
    char *q = p + 2; 
    long rhs = parse_eq(&q, hm); 
    v = v && rhs; 
    p = skipws(q);
  }
  *sp = p; 
  return v;
}

// функция обработки оператора логического ИЛИ
long parse_or(char **sp, HashMap *hm)
{
  long v = parse_and(sp, hm);
  char *p = skipws(*sp);
  while (p[0] == '|' && p[1] == '|') { 
    char *q = p + 2; 
    long rhs = parse_and(&q, hm); 
    v = v || rhs; 
    p = skipws(q); 
  }
  *sp = p; 
  return v;
}

/* 
функция обработки макроса defined
sp - указатель на указатель на текущую позицию в строке
outname - буфер для записи найденного имени
outcap - вместимость буфера для записи найденного имени
*/
int parse_defined_name(char **sp, char *outname, size_t outcap)
{
  char *p = skipws(*sp);
  if (!myStrStr((char*)p, "defined")) //если не "defined", возврат 0
    return 0;

  p += 7;
  p = skipws(p);
  if (*p == '(') {
    p++; p = skipws(p); 
    size_t i = 0;
    while (*p && *p != ')' && i + 1 < outcap) 
      outname[i++] = *p++;

    outname[i] = '\0';
    if (*p == ')') 
      p++;
    *sp = p; 
    return 1; // возвращает 1, записав имя макроса в буфер outname
  }
  size_t i = 0;
  while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_' || (*p >= '0' && *p <= '9')) { 
    if (i + 1 < outcap) 
      outname[i++] = *p; 
    p++; 
  }
  outname[i] = '\0';
  *sp = p; 
  return i > 0; // возвращает 1 если было найдено и записано в имя макроса 
}

// обработка первичных значений
int parse_primary(char **sp, HashMap *hm)
{
  char *p = skipws(*sp);
  if (*p == '(') { // если скобки, наивысший приоритет
    p++; 
    int v = parse_or(&p, hm); 
    p = skipws(p); 
    if (*p == ')') 
      p++; 
    *sp = p; 
    return v; 
  }
  if (myStrStr((char*)p, "defined")) { // если "defined"
    char name[256] = {0}; // буфер для записи имени макроса
    if (parse_defined_name(&p, name, sizeof(name))) { // извлекаем имя
      int res = HMget(hm, name, NULL); // проверка, определено ли имя 
      *sp = p; 
      return res; // возвращаем результат
    }
  }
  if ((*p >= '0' && *p <= '9') || *p == '+' || *p == '-') { //если число
    char *pp = p;
    long num = parse_number(&pp); // обрабатываем как число
    *sp = pp; 
    return num; // возвращаем результат
  }
 
  // проверка на макрос
  char name[256] = {0};
  size_t i = 0;
  while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_' || (*p >= '0' && *p <= '9')) { // извлекаем имя
    if (i + 1 < sizeof(name)) 
      name[i++] = *p;
    p++;
  }
  name[i] = '\0'; 

  if (i > 0 && hm) { // имя прочитано и передана хэш-таблица
    char valbuf[1024] = {0};
    char *valptr = valbuf;
    if (HMget(hm, name, &valptr)) {
      char *pp = skipws(valptr); 
      if (*pp == '+' || *pp == '-' || (*pp >= '0' && *pp <= '9')) { 
        long num = parse_number(&pp);
        *sp = p; 
        return num;
      }
      else {
        char *q = valptr;
        long sub = parse_or(&q, hm);
        *sp = p; 
        return sub;
      }
    }
  }
  *sp = p; 
  return 0;
}

// функция обработки условия
int evalIfExpr(char *s, HashMap *hm)
{
  char *p = s;
  int v = parse_or(&p, hm);
  return v != 0;
}