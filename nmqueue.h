#ifndef __NM__QUEUE__H__
#define __NM__QUEUE__H__
#include "nmlist.h"

typedef nmlist nmqueue;

#define nmqueue_alloc nmlist_alloc;
#define nmqueue_free nmlist_free;

int nmqueue_enqueue(nmqueue *queue, const void *data);
void *nmqueue_dequeue(nmqueue *queue);
int nmqueue_purge(nmqueue *queue);
void *nmqueue_peek(nmqueue *queue);
unsigned int nmqueue_size(nmqueue *queue);

#endif

