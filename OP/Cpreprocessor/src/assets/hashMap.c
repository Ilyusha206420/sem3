#include "hashMap.h"

#include "utils.h"
#include "myString.h"
#include <stddef.h>

// Простая хэш-таблица с списками в корзинах, используется для хранения макросов

// FNV-1a хэш-функция для строк
unsigned long hashFunc (char *str)
{
  unsigned long long hval = 0xcbf29ce484222325ULL;
  while (*str) {
    hval ^= (unsigned long long)(unsigned char)*str++;
    hval *= 0x100000001b3ULL;
  }
  return (unsigned long)hval;
}

// Инициализация хэш-таблицы с заданным количеством корзин
HashMap* HMinit(size_t nop)
{
  HashMap *out = (HashMap*)myAllocMemory(sizeof(HashMap));
  if (!out) return NULL;

  out->map = (HMpocket*)myAllocMemory(sizeof(HMpocket) * nop);
  if (!out->map) { free(out); return NULL; }

  for (size_t i = 0; i < nop; i++) out->map[i] = (HMpocket){NULL, NULL};

  out->numberOfPockets = nop;
  out->numberOfElements = 0;
  return out;
}

// Освобождение всех узлов и структуры
void HMfree(HashMap *hm)
{
  for (size_t i = 0; i < hm->numberOfPockets; i++) {
    HMPnode *node = hm->map[i].begin;
    while (node) {
      hm->map[i].begin = node->next;
      free(node->key);
      free(node->val);
      free(node);
      node = hm->map[i].begin;
    }
  }
  free(hm->map);
  free(hm);
}

// Увеличение размера таблицы (умножение корзин на 2)
void HMresize(HashMap *hm)
{
  size_t newSize = hm->numberOfPockets * 2;
  HMpocket *newMap = (HMpocket*)myAllocMemory(sizeof(HMpocket) * newSize);
  if (!newMap) return;

  for (size_t i = 0; i < newSize; i++) newMap[i] = (HMpocket){NULL, NULL};

  for (size_t i = 0; i < hm->numberOfPockets; i++) {
    HMPnode *node = hm->map[i].begin;
    while (node) {
      HMPnode *next = node->next;
      HMpocket *pocket = &newMap[hashFunc(node->key) % newSize];
      node->next = NULL;
      if (pocket->end) {
        pocket->end->next = node;
        pocket->end = node;
      } else {
        pocket->begin = node;
        pocket->end = node;
      }
      node = next;
    }
  }

  free(hm->map);
  hm->map = newMap;
  hm->numberOfPockets = newSize;
}

// Создать новый узел с копиями строк ключ/значение
HMPnode* HMPNinit(char *key, char *val)
{
  HMPnode *out = (HMPnode*)myAllocMemory(sizeof(HMPnode));
  if (!out) return NULL;

  out->key = (char*)myAllocMemory(myStrGetLen(key) + 1);
  if (!out->key) { free(out); return NULL; }
  out->val = (char*)myAllocMemory(myStrGetLen(val) + 1);
  if (!out->val) { free(out->key); free(out); return NULL; }

  myStrCpy(key, &out->key);
  myStrCpy(val, &out->val);
  out->next = NULL;
  return out;
}

// Найти узел в корзине по ключу
HMPnode* HMPfind(HMpocket *pocket, char *key)
{
  HMPnode *res = pocket->begin;
  while (res) {
    if (myStrCmp(res->key, key)) return res;
    res = res->next;
  }
  return NULL;
}

// Добавление или обновление элемента
int HMadd(HashMap *hm, char *key, char *val)
{
  if (hm->numberOfElements >= (hm->numberOfPockets / 2)) HMresize(hm);

  HMpocket *pocket = &hm->map[hashFunc(key) % hm->numberOfPockets];
  HMPnode *elem = HMPfind(pocket, key);
  if (elem) {
    char *newVal = (char*)myAllocMemory(myStrGetLen(val) + 1);
    if (!newVal) return -1;
    myStrCpy(val, &newVal);
    free(elem->val);
    elem->val = newVal;
  } else {
    HMPnode *newElem = HMPNinit(key, val);
    if (!newElem) return -1;
    if (pocket->end) pocket->end->next = newElem; else pocket->begin = newElem;
    pocket->end = newElem;
    hm->numberOfElements += 1;
  }
  return 0;
}

// Получение значения по ключу
int HMget(HashMap *hm, char *key, char **buf)
{
  HMPnode *node = HMPfind(&hm->map[hashFunc(key) % hm->numberOfPockets], key);
  if (!node) return 0;
  if (buf) myStrCpy(node->val, buf);
  return 1;
}

// Удаление элемента по ключу
void HMdelete(HashMap *hm, char *key)
{
  HMpocket *pocket = &hm->map[hashFunc(key) % hm->numberOfPockets];
  HMPnode *node = pocket->begin;
  HMPnode *prev = NULL;
  while (node && !myStrCmp(node->key, key)) { prev = node; node = node->next; }
  if (!node) return;
  if (pocket->begin == node) pocket->begin = node->next;
  if (pocket->end == node) pocket->end = prev;
  if (prev) prev->next = node->next;
  free(node->key); free(node->val); free(node);
}