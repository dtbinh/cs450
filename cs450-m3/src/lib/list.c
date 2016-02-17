#include <assert.h>
#include "list.h"

void
list_init (struct list *list)
{
  assert (list != NULL);
  list->head.prev = NULL;
  list->head.next = &list->tail;
  list->tail.prev = &list->head;
  list->tail.next = NULL;
}

struct list_elem *
list_begin (struct list *list)
{
  assert (list != NULL);
  return list->head.next;
}

struct list_elem *
list_next (struct list_elem *elem)
{
  assert (elem != NULL);
  return elem->next;
}

struct list_elem *
list_end (struct list *list)
{
  assert (list != NULL);
  return &list->tail;
}

bool
list_empty (struct list *list)
{
  return list_begin (list) == list_end (list);
}

struct list_elem *
list_head (struct list *list)
{
  assert (list != NULL);
  return &list->head;
}

struct list_elem *
list_tail (struct list *list)
{
  assert (list != NULL);
  return &list->tail;
}

void
list_insert (struct list_elem *before, struct list_elem *elem)
{
  assert (before != NULL);
  assert (elem != NULL);
  elem->prev = before->prev;
  elem->next = before;
  before->prev->next = elem;
  before->prev = elem;
}

void
list_push_back (struct list *list, struct list_elem *elem)
{
  list_insert (list_end (list), elem);
}

struct list_elem *
list_remove (struct list_elem *elem)
{
  assert (elem != NULL);
  elem->prev->next = elem->next;
  elem->next->prev = elem->prev;
  return elem->next;
}

struct list_elem *
list_pop_front (struct list *list)
{
  assert (list != NULL);
  struct list_elem *front = list_begin (list);
  list_remove (front);
  return front;
}
