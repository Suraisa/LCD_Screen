#ifndef LIST_CIRCULAR_H
#define LIST_CIRCULAR_H

#include <stdio.h>
#include <stdlib.h>

typedef union
{
    int value;
} LIST_DATA;

typedef struct NODE
{
    struct NODE* next;
    LIST_DATA data;
}*LIST_CIRCULAR;

LIST_CIRCULAR CreateList();

int IsEmpty(LIST_CIRCULAR list);

void AddInFront(LIST_CIRCULAR* list, LIST_DATA value);

void AddAtLast(LIST_CIRCULAR* list, LIST_DATA value);

void ErasedInFront(LIST_CIRCULAR *list);

void ErasedList(LIST_CIRCULAR *list);

#endif