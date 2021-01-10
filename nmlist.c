#include <stdlib.h>
#include "nmlist.h"

struct nmlist_element_s {
	void *data;
	struct nmlist_element_s *next;
};

struct nmlist_s {
	void (*destructor)(void *data);
	int (*cmp)(const void *e1, const void *e2);
	unsigned int size;
	nmlist_element *head;
	nmlist_element *tail;
};

/**
 * Allocates memory for a new linked list.
 *
 * INPUT:
 * 'destructor'		Destructor for 'data' being hold
 * 					in the 'nmlist_element'.
 * RETURNS:
 * A new linked list.
 **/
nmlist *nmlist_alloc(void (*destructor)(void *data))
{
	nmlist *list = NULL;
	if ((list = calloc(1,sizeof(*list))) != NULL) {
		list->size = 0;
		list->destructor = destructor;
		list->head = NULL;
		list->tail = NULL;
	}
	return list;
}

/**
 * De-allocates memory for linked list.
 * 
 * INPUT:
 * 'list'		The linked list to be de-allocated.
 * 
 * RETURN:
 * 0 			If list was succesfuly de-allocated.
 * -1			If something went wrong (list is NULL, 
 * 				destructor is NULL).
 **/
int nmlist_free(nmlist *list)
{
	void *data;
	if(list == NULL || list->destructor == NULL){
		return (-1);
	}
	while(list->size>0){
		if((data = nmlist_remove_next(list, NULL)) != NULL){
			list->destructor(data);
		}
	}
	free(list);
	return (0);
	
}

/**
 * Inserts a new element into the list.
 *
 * If 'element' is NULL, element is inserted
 * at 'list->head'.
 *
 * INPUT:
 * 'list'		List were we operate changes.
 * 'element'	We insert our new element after this one.
 * 'data'		Data to be inserted into the list
 *
 * RETURNS:
 * 0			If insertion was succesful.
 * -1			If the insertion wasn't succesful.
 * 				('list' is NULL, memory allocation failed for new
 * 				element).
 **/
int nmlist_insert_next(nmlist *list, nmlist_element *element, const void *data)
{
	nmlist_element *new_e = NULL;
	new_e = calloc(1, sizeof(*new_e));
	if (list == NULL || new_e == NULL) {
		return (-1);
	}
	new_e->data = (void*) data;
	new_e->next = NULL;
	if (element == NULL) {
		if (list->size == 0) {
			list->tail = new_e;
		}
		new_e->next = list->head;
		list->head = new_e;
	} else {
		if (element->next == NULL) {
			list->tail = new_e;
		}
		new_e->next = element->next;
		element->next = new_e;
	}
	list->size++;
	return (0);
}

/**
 * Inserts a new element into the list at the specified index.
 *
 * INPUT:3
 * 'list'		List were we operate changes.
 * 'index'		Were to insert new element.
 * 'data'		Data to be inserted into the list
 *
 * RETURNS:
 * 0			If insertion was succesful.
 * -1			If the insertion wasn't succesful.
 * 				('list' is NULL, memory allocation failed for new
 * 				element).
 **/
int nmlist_insert_index(nmlist *list, unsigned int index, const void *data)
{
	int i;
	nmlist_element *tmp = NULL;
	if (list == NULL || index > list->size) {
		return (-1);
	}
	if (index == 0) {
		nmlist_insert_next(list, NULL, data);
	} else {
		for (i = 0, tmp = list->head; i < index; i++) {
			tmp = tmp->next;
		}
		nmlist_insert_next(list, tmp, data);
	}
	return (0);
}

/**
 * Removes and returns element from the list.
 *
 * If element is NULL the head is removed.
 *
 * INPUT:
 * 'list'		List were we operate changes.
 * 'element'	The new data will be removed from 'element->next'.
 *
 * RETURNS:
 * NULL			If list is NULL, or empty.
 * 'data'		The data retrieved from the list.
 **/
void *nmlist_remove_next(nmlist *list, nmlist_element *element)
{
	void *data = NULL;
	nmlist_element *old_e = NULL;
	if (list == NULL || list->size == 0) {
		return NULL;
	}
	if (element == NULL) {
		data = list->head->data;
		old_e = list->head;
		list->head = list->head->next;
		if (list->size == 1) {
			list->tail = NULL;
		}
	} else {
		if (element->next == NULL) {
			return NULL;
		}
		data = element->next->data;
		old_e = element->next;
		element->next = old_e->next;
		if (element->next == NULL) {
			list->tail = element;
		}
	}
	free(old_e);
	list->size--;
	return data;
}

/**
 * Removes the 'nth' element from the list.
 *
 * INPUT:
 * 'list'		List were we operate changes.
 * 'index'		The index of the element to be removed.
 *
 * RETURNS:
 * NULL			If list is NULL, or empty.
 * 'data'		The data retrieved from the list.
 **/
void *nmlist_remove_index(nmlist *list, unsigned int index)
{
	int i;
	nmlist_element *tmp = NULL;
	void *data = NULL;
	if (list == NULL || index > list->size) {
		return NULL;
	}
	if (index == 0) {
		data = nmlist_remove_next(list, NULL);
	} else {
		for (i = 0, tmp = list->head; i < index - 1; i++) {
			tmp = tmp->next;
		}
		data = nmlist_remove_next(list, tmp);
	}
	return (data);
}

/**
 * Removes data from the list after the specified
 * element.
 *
 * If 'element' is NULL the head will be purged.
 *
 * Data contained by the removed element
 * is 'purged' (memory free - destructor call).
 *
 * Returns:
 * 0		If the element is succesfuly purged.
 * -1		If the element isn't purged .
**/
int nmlist_purge_next(nmlist *list, nmlist_element *element)
{
	void *data = NULL;
	if (list == NULL || list->destructor == NULL) {
		return (-1);
	}
	data = nmlist_remove_next(list, element);
	if (data != NULL) {
		list->destructor(data);
	}
	return (0);
}

/**
 * Removes the index from the list.
 *
 * Data contained by the removed element
 * is 'purged' (memory free - destructor call).
 *
 * Returns:
 * 0		If the element is succesfuly purged.
 * -1		If the element isn't purged .
**/
int nmlist_purge_index(nmlist *list, unsigned int index)
{
	void *data;
	if (list == NULL || index > list->size
	        || list->destructor == NULL) {
		return (-1);
	}
	data = nmlist_remove_index(list, index);
	if (data != NULL) {
		list->destructor = NULL;
	}
	return (0);
}

/**
 * Returns 'list->size'.
 * 
 * If list is NULL it returns '0' to avoid 
 * segmentantion fault.
 **/
unsigned int nmlist_size(nmlist *list)
{
	return (list == NULL) ? 0 : list->size;
}

/**
 * Returns 'list->head'.
 **/
nmlist_element *nmlist_head(nmlist *list)
{
	return (list != NULL) ? list->head : NULL;
}

/**
 * Returns 'list->tail'.
 **/
nmlist_element *nmlist_tail(nmlist *list)
{
	return (list != NULL) ? list->tail : NULL;
}

/**
 * Returns 'element->next'.
 **/
nmlist_element *nmlist_next(nmlist_element *element)
{
	return (element != NULL) ? element->next : NULL;
}

/**
 * Returns the 'index'th element from the list.
 **/
nmlist_element *nmlist_index(nmlist *list, unsigned int index)
{
	int i;
	nmlist_element *tmp = NULL;
	if (list == NULL || index > list->size) {
		return NULL;
	}
	for (i = 0, tmp = list->head; i < index; i++) {
		tmp = tmp->next;
	}
	return tmp;
}

/**
 * Retrives a pointer to 'data' contained
 * by the list element.
 *
 * INPUT:
 * 'element'		List element.
 *
 * RETURNS:
 * NULL				If element is NULL or it doesn't
 * 					contain any data.
 * 'data'			A pointer to 'element->data'.
 **/
void *nmlist_get_data(nmlist_element *element)
{
	return (element == NULL) ? NULL : element->data;
}

/**
 * Retrieves a pointer to 'data' contained
 * by 'list->head'.
 *
 * INPUT:
 * 'list'			The linked list.
 *
 * RETURNS:
 * NULL				If list is NULL, empty or 'list->head'
 * 					doesn't contain any data.
 * 'data'			If retrieval is succesful.
 **/
void *nmlist_get_head(nmlist *list)
{
	return (list == NULL || list->head == NULL) ? NULL : list->head->data;
}


/**
 * Retrieves a pointer to 'data' contained
 * by 'list->tail'.
 *
 * INPUT:
 * 'list'			The linked list.
 *
 * RETURNS:
 * NULL				If list is NULL, empty or 'list->tail'
 * 					doesn't contain any data.
 * 'data'			If retrieval is succesful.
 **/
void *nmlist_get_tail(nmlist *list)
{
	return (list == NULL || list->tail == NULL) ? NULL : list->tail->data;
}

/**
 * Retrieves a pointer do 'data' contained
 * by 'element->next' ('element->next->data').
 *
 * INPUT:
 * 'element'		List element.
 *
 * RETURNS:
 * NULL			If 'element', 'element->next' or 'data' are NULL.
 * 'data'			If retrieval is succesful.
 **/
void *nmlist_get_next(nmlist_element *element)
{
	return (element == NULL || element->next == NULL) ? NULL : element->data;
}

/**
 * Retrieves a pointer to 'data' from
 * the 'index'th element contained by the linked list.
 *
 * INPUT:
 * 'list'			The linked list.
 * 'index'			Index of the element.
 *
 * RETURNS:
 * NULL			If list is NULL, index out of bounds, or
 * 				'data' is NULL.
 * 'data'			If retrieval is succesful.
 **/
void *nmlist_get_index(nmlist *list, unsigned int index)
{
	nmlist_element *element;
	return ((element = nmlist_index(list, index)) == NULL) ? NULL : element->data;
}

/**
 * Returns list->destructor.
 **/
void (*nmlist_get_destructor(nmlist *list))(void *data)
{
	return list->destructor;
}

/**
 * Sets 'element->data' to data.
 *
 * INPUT:
 * 'element'		Linked list element.
 * 'data'			New data.
 *
 * Returns:
 * 0				If 'data' was succesfuly updated.
 * -1				If not.
 **/
int nmlist_set_data(nmlist_element *element, const void *data)
{
	if (element == NULL) {
		return (-1);
	}
	element->data = (void*) data;
	return (0);
}

/**
 * Sets 'list->head->data' to data.
 *
 * INPUT:
 * 'list'			Linked list element.
 * 'data'			New data.
 *
 * Returns:
 * 0				If 'data' was succesfuly updated.
 * -1				If not.
 **/
int nmlist_set_head(nmlist *list, const void *data)
{
	if (list == NULL) {
		return (-1);
	}
	return nmlist_set_data(list->head, data);
}

/**
 * Sets 'list->tail->data' to data.
 *
 * INPUT:
 * 'list'			Linked list element.
 * 'data'			New data.
 *
 * Returns:
 * 0				If 'data' was succesfuly updated.
 * -1				If not.
 **/
int nmlist_set_tail(nmlist *list, const void *data)
{
	if (list == NULL) {
		return (-1);
	}
	return nmlist_set_data(list->tail, data);
}

/**
 * Sets 'element->next->data' to data.
 *
 * INPUT:
 * 'element'		Linked list element.
 * 'data'			New data.
 *
 * Returns:
 * 0				If 'data' was succesfuly updated.
 * -1				If not.
 **/
int nmlist_set_next(nmlist_element *element, const void *data)
{
	if (element == NULL) {
		return (-1);
	}
	return nmlist_set_data(element->next, data);
}

/**
 * Sets 'element->next->data' to data.
 *
 * INPUT:
 * 'element'		Linked list element.
 * 'data'			New data.
 *
 * Returns:
 * 0				If 'data' was succesfuly updated.
 * -1				If not.
 **/
int nmlist_set_index(nmlist *list, unsigned int index, const void *data)
{
	nmlist_element *element = NULL;
	if (list == NULL ||
	        (element = nmlist_index(list, index)) == NULL) {
		return (-1);
	}
	return nmlist_set_data(element, data);
}

/**
 * Sets 'list->destructor'.
 * 
 * INPUT:
 * 'list'			The linked list.
 * 'destructor'		New destructor function.
 * 
 * RETURN:
 * 0				If destructor is succesfuly updated.
 * -1				If destructor is not succesfuly updated.
 **/
int nmlist_set_destructor(nmlist *list, void(*destructor)(void *data))
{
	if(list == NULL){
		return (-1);
	}
	list->destructor = destructor;
	return (0);
}
