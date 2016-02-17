#ifndef __LIB_LIST_H__
#define __LIB_LIST_H__

#include <stdbool.h>
#include <stdio.h>

struct list_elem
  {
    struct list_elem *prev;
    struct list_elem *next;
  };

struct list
  {
    struct list_elem head;
    struct list_elem tail;
  };

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *) 0)->MEMBER)

#define list_entry(LIST_ELEM, STRUCT, MEMBER)			\
	((STRUCT *) ((uint8_t *) &(LIST_ELEM)->next		\
			- offsetof (STRUCT, MEMBER.next)))

void list_init (struct list *list);
struct list_elem * list_begin (struct list *list);
struct list_elem * list_next (struct list_elem *elem);
struct list_elem * list_end (struct list *list);
bool list_empty (struct list *list);
struct list_elem * list_head (struct list *list);
struct list_elem * list_tail (struct list *list);
void list_insert (struct list_elem *before, struct list_elem *elem);
void list_push_back (struct list *list, struct list_elem *elem);
struct list_elem * list_remove (struct list_elem *elem);
struct list_elem * list_pop_front (struct list *list);

#endif
