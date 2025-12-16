#pragma once

#include <stdlib.h>

// элемент кармана хэш таблицы
typedef struct HMPnode
{
  char *key;
  char *val;
  struct HMPnode *next;
} HMPnode;

// карман хэш таблицы
typedef struct 
{
  HMPnode *begin;
  HMPnode *end;
} HMpocket;

// хэш таблица
typedef struct 
{
  HMpocket *map;
  size_t numberOfPockets;
  size_t numberOfElements;
} HashMap;

// инициализация пустой хэш таблицы, nop - количество карманов
HashMap* HMinit(size_t nop);
// освобождение выделенной памяти для хэш таблицы
void HMfree(HashMap *hm);

// добавление элемента в хэш таблицу
int HMadd(HashMap *hm, char *key, char *val);
// получение элемента хэш таблицы по ключу
int HMget(HashMap *hm, char *key, char **buf);
// удаление элемента хэш таблицы по ключу
void HMdelete(HashMap *hm, char *key);