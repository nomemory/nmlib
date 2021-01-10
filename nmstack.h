#ifndef __NM__STACK__H__
#define __NM__STACK__H__
#include "nmlist.h"

typedef nmlist nmstack;

#define nmstack_alloc nmlist_alloc
#define nmstack_free nmlist_free

int nmstack_push(nmstack *stack, const void *data);
void *nmstack_pop(nmstack *stack);
int nmstack_purge(nmstack *stack);
void *nmstack_peek(nmstack *stack);
unsigned int nmstack_size(nmstack *stack);

#endif