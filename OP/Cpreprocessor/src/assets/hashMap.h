#pragma once

#include <stdlib.h>

// Узел в списке элементов (ключ->значение)
typedef struct HMPnode
{
  char *key;       // ключ (строка)
  char *val;       // значение (строка)
  struct HMPnode *next; // указатель на следующий узел
} HMPnode;

// Карман (bucket) хэш-таблицы — двусвязный список через begin/end
typedef struct 
{
  HMPnode *begin; // первый элемент в корзине
  HMPnode *end;   // последний элемент в корзине
} HMpocket;

// Основная структура хэш-таблицы
typedef struct 
{
  HMpocket *map;            // массив карманов
  size_t numberOfPockets;   // размер массива
  size_t numberOfElements;  // текущее число элементов
} HashMap;

// Инициализация пустой хэш-таблицы с заданным числом карманов.
// Возвращает NULL при ошибке выделения памяти.
HashMap* HMinit(size_t nop);

// Освобождение памяти, занятой хэш-таблицей.
void HMfree(HashMap *hm);

// Добавление или обновление элемента (ключ, значение).
// Возвращает 0 при успехе, -1 при ошибке.
int HMadd(HashMap *hm, char *key, char *val);

// Получение значения по ключу. Если найдено и `buf` != NULL, копируем значение в `*buf`.
// Возвращает 1 если найдено, 0 если нет.
int HMget(HashMap *hm, char *key, char **buf);

// Удаление элемента по ключу (если есть).
void HMdelete(HashMap *hm, char *key);