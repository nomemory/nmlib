#include <stdlib.h>
#include "nmstack.h"

/** 
 * Pushes 'data' onto the stack.
 * 
 * INPUT:
 * 'stack'			The stack.
 * 'data'			The data to be pushed.
 * 
 * RETURNS:
 * 0				If the data is succesfuly pushed.
 * -1				If something went wrong.
 **/
int nmstack_push(nmstack *stack, const void *data)
{
	return nmlist_insert_next(stack, NULL, data);
}

/**
 * Pops 'data' from the stack.
 * 
 * INPUT:
 * 'stack'			The stack.
 * 
 * RETURNS:
 * NULL				If the data is NULL or something went wrong.
 * 'data'			If the data is succesfuly popped out.
 **/ 
void *nmstack_pop(nmstack *stack)
{
	return nmlist_remove_next(stack, NULL);
}

/**
 * Pops 'head' from the stack, and purges 
 * the data.
 * 
 * INPUT:
 * 'stack'			The stack.
 * 
 * RETURN:
 * 0				If the data from stack->head is succesfuly purged.
 * -1				If something went wrong.
 **/
int nmstack_purge(nmstack *stack)
{
	return nmlist_purge_next(stack, NULL);
}

/**
 * Returns 'head->data' without removing it.
 * 
 * INPUT:
 * 'stack'			The stack.
 * 
 * RETURNS:
 * 'data'			The data contained by the head.
 * NULL				If something went wrong.
 **/
void *nmstack_peek(nmstack *stack)
{
	return nmlist_get_data(nmlist_head(stack));
}

/**
 * Returns 'queue->size' .
 * 
 * (If queue is NULL, it returns 0, to avoid
 * 'segmentantion fault')
 * 
 * RETURNS:
 * 'queue->size'
 **/
unsigned int nmstack_size(nmstack *stack)
{
	return nmlist_size(stack);
}

