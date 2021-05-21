#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _LIST_ELEMENT{
    char *string;
    struct _LIST_ELEMENT *next;
}List_element;

typedef struct _LIST{
    struct _LIST_ELEMENT *head;
}List;

List *list_init(void); // Создаёт список
int list_insert(List *list, char *string, int index); // Вставляет новый элемент по индексу
int list_remove(List *list, int index); // Удаляет элемент по индексу
int list_take(List *list, int index, char **ret); // Берёт значение элемента по индексу и записывает его в *ret
int list_len(List* list); // Возвращает длину списка
int list_write(List *list); // Выводит весь список
int list_clear(List *list); // Удаляет весь список

#endif //LIST_H
