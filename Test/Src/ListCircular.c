#include "ListCircular.h"

LIST_CIRCULAR CreateList()
{
    return NULL;
}

int IsEmpty(LIST_CIRCULAR list)
{
    return !list;
}

void AddInFront(LIST_CIRCULAR* list, LIST_DATA value)
{
    LIST_CIRCULAR newNode = CreateList();
    newNode = (LIST_CIRCULAR)calloc(1, sizeof(*newNode));

    if(!newNode)
        return;

    newNode->data = value;

    if(IsEmpty(*list))
    {
        *list = newNode;
        newNode->next = newNode;
    }
    else
    {
        newNode->next = (*list)->next;
        (*list)->next = newNode;
    }
}

void AddAtLast(LIST_CIRCULAR* list, LIST_DATA value)
{
    LIST_CIRCULAR newNode = CreateList();
    newNode = (LIST_CIRCULAR)calloc(1, sizeof(*newNode));

    if(!newNode)
        return;

    newNode->data = value;

    if(IsEmpty(*list))
    {
        *list = newNode;
        newNode->next = newNode;
    }
    else
    {
        newNode->next = (*list)->next;
        (*list)->next = newNode;
        *list = newNode;
    }
}

void ErasedInFront(LIST_CIRCULAR *list)
{
    if (IsEmpty(*list))
        return;

    LIST_CIRCULAR nextNode = (*list)->next->next;
    if(*list != (*list)->next)
    {
        free((*list)->next);
        (*list)->next = nextNode;
    }
    else
    {
        free((*list)->next);
        *list = NULL;
    }
}

void ErasedList(LIST_CIRCULAR *list)
{
    LIST_CIRCULAR nodeI;
    LIST_CIRCULAR nodeISuiv;

    for (nodeI = *list; !IsEmpty(nodeI); nodeI = nodeISuiv)
    {
        nodeISuiv = nodeI->next;
        free(nodeI);
    }
    *list = NULL;
}