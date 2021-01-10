#include <stdio.h>
#include <stdlib.h>
#include "nmaux.h"
#include "nmvect.h"

struct nmvect_s {
	void (*destructor)(void *data);
	int (*cmp)(const void *e1, const void *e2);
	unsigned int capacity;
	unsigned int size;
	nmvect_element *array;
};

struct nmvect_element_s {
	void *data;
};

/**
 * Allocates memory for a new empty 'vect'.
 *
 * INPUT:
 * 'destructor'		Function needed to free memory & data
 * 					associated with the vector.
 * 'cmp'			Function needed to compare two elements from
 * 					the vector.
 * 					RETURNS:
 * 					0		if *e1 == *e2
 * 					1		if *e1 > *e2
 * 					-1		if *e1 < *e2
 *
 **/
nmvect *nmvect_alloc(unsigned int icap, void (*destructor)(void *data),
                     int (*cmp)(const void *e1, const void *e2))
{
	nmvect *vect = NULL;
	vect = calloc(1, sizeof(*vect));
	if (vect == NULL) {
		return NULL;
	}
	vect->capacity = icap;
	vect->array = calloc(icap, sizeof(*vect->array));
	if (vect->array == NULL) {
		free(vect);
		return NULL;
	}
	vect->size = 0;
	vect->destructor = destructor;
	vect->cmp = cmp;
	return vect;
}

/**
 * De-allocates memory for 'vect'
 *
 * INPUT:
 * 'vect'			The vector to be allocated.
 *
 * RETURNS:
 * 0				If memory de-allocation was succesful.
 * -1				If vect is NULL, or destructor is NULL.
 **/
int nmvect_free(nmvect *vect)
{
	int i;
	if (vect == NULL || vect->destructor == NULL) {
		return (-1);
	}
	for (i = 0; i < vect->size ; i++) {
		if (vect->array[i].data != NULL) {
			vect->destructor(vect->array[i].data);
		}
	}
	free(vect->array);
	free(vect);
	return (0);
}

/**
 * Expands 'vect' capacity.
 * New capacity will be 'vect->capacity*3/2+1'.
 *
 * INPUT:
 * 'vect'			The vector.
 *
 * RETURNS
 * 0				If capacity was succesfuly expanded.
 * -1				If capacity wasn't succesfuly expanded.
 * 					(memory re-allocation failed)
 **/
int nmvect_expand(nmvect *vect)
{
	unsigned int tmp_cap;
	nmvect_element *tmp_array;
	if (vect == NULL) {
		return (-1);
	}
	tmp_cap = vect->capacity * 3 / 2 + 1;
	tmp_array = realloc(vect->array, tmp_cap * sizeof(nmvect_element));
	if (tmp_array != NULL) {
		vect->array = tmp_array;
		vect->capacity = tmp_cap;
		return (0);
	}
	return (-1);
}

/**
 * Contracts 'vect' capacity.
 * New capacity will be 'vect->capacity*2/3+1'.
 *
 * INPUT:
 * 'vect'			The vector.
 *
 * RETURNS
 * 0				If capacity was succesfuly contracted.
 * -1				If capacity wasn't succesfuly contracted.
 * 					(memory re-allocation failed)
 **/
int nmvect_contract(nmvect *vect)
{
	unsigned int tmp_cap;
	nmvect_element *tmp_array;
	if (vect == NULL) {
		return (-1);
	}
	tmp_cap = vect->capacity * 2 / 3 + 1;
	tmp_array = realloc(vect->array, tmp_cap * sizeof(nmvect_element));
	if (tmp_array != NULL) {
		vect->array = tmp_array;
		vect->capacity = tmp_cap;
		return (0);
	}
	return (-1);
}

/**
 * Modifies 'vect' with a given modificator 'modif'.
 * New capacity for 'vect' will be 'vect->capacity +  modif'.
 *
 * If 'modif' is negative, the new capacity will shrink.
 *
 * If 'vect->capacity+modif < 1' the function will return (-1);
 * Else the function will return (0);
 **/
int nmvect_modcap(nmvect *vect, int modif)
{
	unsigned int tmp_cap;
	nmvect_element *tmp_array;
	if (vect == NULL || vect->capacity + modif < 1) {
		return (-1);
	}
	if (modif == 0) {
		return (0);
	}
	tmp_cap = vect->capacity + modif;
	tmp_array = realloc(vect->array, tmp_cap * sizeof(nmvect_element));
	if (tmp_array != NULL) {
		vect->array = tmp_array;
		vect->capacity = tmp_cap;
		return (0);
	}
	return (-1);
}

/**
 * Inserts the specified data at the 'index'th position.
 *
 * If an array {a0, a1, a2, a3, a4, ..., aN} has an element inserted
 * at position '3' the new array after insertion will become:
 * {a0, a1, a2, aINSERTED, a3, a4, ..., aN} .
 *
 * INPUT:
 * 'vect'		The vector.
 * 'index'		Index where to insert the new element.
 * 'data'		Data to be inserted.
 *
 * RETURNS:
 * 0			If insertion was succesful.
 * -1			If insertion wasn't succesful. (index is out of bounds,
 * 				'vect' is NULL.
 **/
int nmvect_insert(nmvect *vect, unsigned int index, const void *data)
{
	int i;
	unsigned int ilim;
	void *tmp, *aux;
	if (vect == NULL || index > vect->size) {
		return (-1);
	}
	if (index == vect->size) {
		nmvect_append(vect, data);
	} else {
		if (vect->size == vect->capacity) {
			nmvect_expand(vect);
		}
		ilim = vect->size + 1;
		for (i = index, tmp = (void*) data; i < ilim; i++) {
			aux = vect->array[i].data;
			vect->array[i].data = tmp;
			tmp = aux;
		}
	}
	vect->size++;
	return (0);
}

/**
 * Inserts 'addvect' array at the specified position.
 *
 * Given 'vect' = {a0, a1, a2, a3, a4, ... aN } and
 * 'addvect' = {b0, b1, b2, b3, b4, ... bN } after insertion at position
 * 3 for example :
 * 'vect' = {a0, a1, a2, b0, b1, b2, ..., bN, a3, a4, ..., aN}.
 *
 * INPUT:
 * 'vect'		The vector where to add elements.
 * 'index'		The index where to insert 'addvect'.
 * 'addvect'	The vector to be inserted.
 *
 * RETURNS:
 * 0			If insertion was succesful.
 * -1			If insertion wasn't succesful.
 **/
int nmvect_insert_range(nmvect *vect, unsigned int index, nmvect *addvect)
{
	int i;
	nmvect *moved_data;
	if (vect == NULL ||
	        addvect == NULL ||
	        index > vect->size ||
	        nmvect_modcap(vect, vect->capacity + addvect->size) != 0 ||
	        (moved_data = nmvect_alloc(addvect->size, vect->destructor, NULL)) == NULL) {
		return (-1);
	}
	for (i = index; i < vect->size; i++) {
		if (nmvect_append(moved_data, (const void*)vect->array[i].data) != 0) {
			free(moved_data->array);
			free(moved_data);
			return (-1);
		}
	}
	for (i = 0; i < addvect->size; i++) {
		if (i + index >= vect->size) {
			vect->size++;
		}
		vect->array[i+index].data = addvect->array[i].data;
	}
	for (i = 0; i < moved_data->size; i++) {
		/* At this point append cannot fail. The capacity is big enough
		 * and was previously expanded. No reason to check.*/
		nmvect_append(vect, (const void*) nmvect_get(moved_data, i));
	}

	free(moved_data->array);
	free(moved_data);

	return (0);
}

/**
 * Appends element to 'vect'.
 *
 * INPUT:
 * 'vect'		Vector where to append new element.
 * 'data'		Data to be appended.
 *
 * RETURNS:
 * 0			If insertion was succesfu..
 * -1
 **/
int nmvect_append(nmvect *vect, const void *data)
{
	if (vect == NULL) {
		return (-1);
	}
	if (vect->size ==  vect->capacity) {
		if (nmvect_expand(vect) != 0) {
			return (-1);
		}
	}
	vect->array[vect->size].data = (void*) data;
	vect->size++;
	return(0);
}

/**
 * Appends 'appvect' to 'vect'.
 *
 * INPUT:
 * 'vect'		Vector where to append.
 * 'appvect'	Vector to be appended.
 *
 * RETURNS:
 * 0		If operation was succesful.
 * -1		If operation wasn't succesful.
 **/
int nmvect_append_range(nmvect *vect, unsigned int index, nmvect *appvect)
{
	return nmvect_insert_range(vect, vect->size, appvect);
}

/**
 * Test if 'data' is contained inside the 'vect'.
 *
 * Vector should have the comparator function != NULL.
 * The function doesn't tell at which position the data is
 * found.
 *
 * INPUT:
 * 'vect'		Vector where to look for 'data'
 * 'data'		Data be looked for.
 *
 * RETURNS:
 * 1			If vector contains data.
 * 0			If vector do not contains the data.
 * -1			If search operation wasn't succesful.
 **/
int nmvect_contains(nmvect *vect, const void *data)
{
	int i;
	if (vect == NULL || vect->cmp == NULL) {
		return (-1);
	}
	for (i = 0; i < vect->size; i++) {
		if (vect->cmp((const void*) vect->array[i].data, data) == 0) {
			return (0);
		}
	}
	return (-1);
}

/**
 * Returns an 'array' of integers.
 * Every integer points to the a position in 'vect'
 * that holds 'data'.
 *
 * INPUT:
 * 'vect'		The array where to look for objects.
 * 'data'		The data to look for.
 * 'occ_s'		The returning array size.
 *
 * OUTPUT:
 * An array containing all the occurences of the array.
 **/
nmlist *nmvect_occurence(nmvect *vect, const void *data)
{
	nmlist *rlist = NULL;
	int i, *j;
	if (vect == NULL) {
		return NULL;
	}
	rlist = nmlist_alloc(nmaux_primitive_destructor);
	for (i = 0; i < vect->size; i++) {
		if (vect->cmp((const void*)vect->array[i].data, data) == 0) {
			*(j = malloc(sizeof(*j))) = i;
			if (nmlist_insert_next(rlist, nmlist_tail(rlist), (const void*) j)!=0) {
				nmlist_free(rlist);
				return NULL;
			}
		}
	}
	return rlist;
}

/**
 * Returns data contained at the specified index.
 *
 * INPUT:
 * 'vect'		The vector.
 * 'index'		The index.
 *
 * RETURNS:
 * 'data'		If operation was succesful.
 * NULL			If operation wasn't succesful
 **/
void *nmvect_get(nmvect *vect, unsigned int index)
{
	if (vect == NULL || index >= vect->size) {
		return NULL;
	}
	return vect->array[index].data;
}

/**
 * Updates the data contained by the specified index.
 *
 * INPUT:
 * 'vect'		The vector.
 * 'index'		The index where we update the data.
 * 'data'		New data.
 **/
int nmvect_set(nmvect *vect, unsigned int index, const void *data)
{
	if (vect==NULL || index >= vect->size) {
		return (-1);
	}
	vect->array[index].data = (void*) data;
	return (0);
}

/**
 * Removes the current 'index' from 'vect', and returns the 'data'
 * contained by the 'index'.
 *
 * INPUT:
 * 'vect'		The vector.
 * 'index'		Index to be removed.
 *
 * RETURNS:
 * NULL			If index is out of bounds, 'vect' is NULL.
 * 'data'		Data contained by the element.
 **/
void *nmvect_remove(nmvect *vect, unsigned int index)
{
	void *data;
	int i;
	if (vect == NULL || index >= vect->size) {
		return NULL;
	}
	if (vect->size == vect->capacity * 2 / 3 + 1) {
		/* Eventually contract the vector capacity */
		nmvect_contract(vect);
	}
	data = vect->array[index].data;
	vect->size--;
	for (i = index; i < vect->size; i++) {
		vect->array[i].data = vect->array[i+1].data;
	}
	return (data);
}

/**
 * Removes a "slice" of the vector starting from 'start' index
 * until the 'stop' index.
 *
 * INPUT:
 * 'vect'		The vector where you apply removal.
 * 'start'		Starting index.
 * 'stop'		Stop index.
 *
 * RETURNS:
 * 'nmvect'		A vector containing the removed elements.
 * NULL			If the operation is not succesful.
 * 				('vect' is NULL, indexes out of bounds,
 * 					'start' bigger than 'stop', etc.)
 **/
nmvect *nmvect_remove_range(nmvect *vect, unsigned int start, unsigned int stop)
{
	nmvect *rvect = NULL;
	int dif;
	int i, ilim;
	if (vect == NULL ||
	        start >= vect->size ||
	        stop > vect->size ||
	        (stop - start) <= 0 ||
	        (rvect = nmvect_alloc((stop-start), vect->destructor, vect->cmp)) == NULL) {
		return NULL;
	}
	/* Generating response */
	for (i = start; i < stop; i++) {
		if (nmvect_append(rvect, vect->array[i].data) != 0) {
			free(rvect->array);
			free(rvect);
			return NULL;
		}
	}
	/* Removing elements */
	dif = stop - start;
	ilim = vect->size - dif;
	for (i = start; i < ilim; i++) {
		vect->array[i].data = vect->array[i+dif].data;
	}
	nmvect_modcap(vect, -dif);
	vect->size -= dif;
	return rvect;
}

/**
 * Removes 'index'th element from the vector
 * and frees data.
 *
 * INPUT:
 * 'vect'		The vector.
 * 'index'		Indicate the element you want to purge.
 *
 * RETURNS:
 * 0			If purge was succesful.
 * -1			If something went wrong.
 **/
int nmvect_purge(nmvect *vect, unsigned int index)
{
	void *data = NULL;
	if (vect == NULL ||
	        index >= vect->size) {
		return (-1);
	}
	vect->destructor(data);
	return (0);
}

/**
 * Remove a range from the vector and frees the
 * associated data.
 *
 * INPUT:
 * 'vect'		The vector to be modified.
 * 'start'		Starting point for removal.
 * 'stop'		Stopping point for removal.
 *
 * RETURNS:
 * 0			If purge was succesful.
 * -1			If purge wasn't succesful.
 **/
int nmvect_purge_range(nmvect *vect, unsigned int start, unsigned int stop)
{
	nmvect *rvect;
	if (vect == NULL ||
	        start >= vect->size ||
	        stop > vect->size ||
	        stop - start <= 0) {
		return (-1);
	}
	rvect = nmvect_remove_range(vect, start, stop);
	nmvect_free(rvect);
	return (0);
}

/**
 * Returns vector capacity.
 **/
unsigned int nmvect_capacity(nmvect *vect)
{
	return vect->capacity;
}

/**
 * Returns vector size.
 **/
unsigned int nmvect_size(nmvect *vect)
{
	return vect->size;
}
