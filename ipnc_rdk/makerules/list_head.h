#ifndef _LIST_H
#define _LIST_H

#include <unistd.h>
#include <stddef.h>

struct list_head {
	struct list_head *next,*prev;
};

//#define offsetof(TYPE,MEMBER)	((size_t)&((TYPE *)0)->MEMBER)
#define container_of(ptr,type,member)	({	\
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );} )

#define LIST_HEAD_INIT(name)  { &(name), &(name) }
#define LIST_HEAD(name)		\
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *cur,	
			struct list_head *prev,
			struct list_head *next)
{
	cur->prev = prev;
	cur->next = next;
	next->prev = cur;
	prev->next = cur;
}

static inline void list_add(struct list_head *cur,struct list_head *head)
{
	__list_add(cur,head,head->next);
}

static inline void list_add_tail(struct list_head *cur,struct list_head *head)
{
	__list_add(cur,head->prev,head);
}

static inline void __list_del(struct list_head *prev,struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev,entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static inline int list_emptry_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}


static inline struct list_head *list_get_tail(struct list_head *head)
{
	return head->prev;
}


#define list_entry(ptr,type,member)	\
	container_of(ptr,type,member)

#define list_get_entry(pos,head,member)	\
	 pos = list_entry((head)->next,typeof(*pos),member)

#define list_for_each(pos,head)	\
	for(pos = (head)->next;pos != (head);pos = pos->next)

#define __list_for_each(pos,head)	\
	for(pos=(head)->next;pos != (head);pos = pos->next)

#define list_for_each_prev(pos,head)	\
	for(pos = (head)->prev;pos != (head);pos = pos->prev)

#define list_for_each_safe(pos,n,head)	\
	for(pos = (head)->next,n = pos->next; pos != (head);	\
		pos = n, n = pos->next)

#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
	n = list_entry(pos->member.next, typeof(*pos), member);	\
	&pos->member != (head); 					\
	pos = n, n = list_entry(n->member.next, typeof(*n), member))

#define list_for_each_entry(pos,head,member)	\
	for(pos = list_entry((head)->next,typeof(*pos),member); \
		&pos->member != (head);	\
		pos = list_entry(pos->member.next,typeof(*pos),member) )

#define list_for_each_entry_reverse(pos,head,member)	\
	for(pos=list_entry((head)->prev,typeof(*pos),member);	\
		pos->member != (head);	\
		pos = list_entry(pos->member.prev,typeof(*pos),member) )



#endif

