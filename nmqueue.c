#include <stdlib.h>
#include "nmqueue.h"

/**
 * Enqueue 'data' into the queue.
 * (Inserts at tail).
 * 
 * INPUT:
 * 'data'			The data to be inserted.
 * 'queue'			The queue.
 *
 * RETURNS:
 * 0				If insertion was succesful.
 * -1				If something went wrong.
 **/
int nmqueue_enqueue(nmqueue *queue, const void *data)
{
	return nmlist_insert_next(queue, nmlist_tail(queue), data);
}

/**
 * Dequeue 'data' from the queue.
 * (Removes head).
 * 
 * INPUT:
 * 'queue'			The queue.
 * 
 * RETURNS:
 * NULL				If data is NULL, queue is NULL or empty.
 * 'data'		
 **/
void *nmqueue_dequeue(nmqueue *queue)
{
	return nmlist_remove_next(queue, NULL);
}


/**
 * Dequeue & purge 'data' from the queue.
 * (Removes & purge head).
 * 
 * INPUT:
 * 'queue'			The queue.
 * 
 * RETURNS:
 * 0				If purge action was succesful.
 * -1				If something went wrong.
 **/
int nmqueue_purge(nmqueue *queue)
{
	return nmlist_purge_next(queue, NULL);
}

/**
 * Returns data that is being hold
 * in the 'head' of the queue.
 * 
 * INPUT:
 * 'queue'			The queue.
 * 
 * RETURNS:
 * 'data'
 * NULL
 **/
void *nmqueue_peek(nmqueue *queue)
{
	return nmlist_get_data(nmlist_head(queue));
}

/**
 * Returns 'queue' actual size.
 * 
 * INPUT:
 * 
 **/
unsigned int nmqueue_size(nmqueue *queue)
{
	return nmlist_size(queue);
}