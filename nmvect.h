#ifndef __NM__VECT__H__
#define __NM__VECT__H__

#include "nmlist.h"
#include "nmaux.h"

typedef struct nmvect_element_s nmvect_element;
typedef struct nmvect_s nmvect;

nmvect *nmvect_alloc(unsigned int icap, void (*destructor)(void *data), int (*cmp)(const void *e1, const void *e2));
int nmvect_free(nmvect *vect);
int nmvect_modcap(nmvect *vect, int modif);
int nmvect_expand(nmvect *vect);
int nmvect_contract(nmvect *vect);
int nmvect_insert(nmvect *vect, unsigned int index, const void *data);
int nmvect_insert_range(nmvect *vect, unsigned int index, nmvect *addvect);
int nmvect_append(nmvect *vect, const void *data);
int nmvect_append_range(nmvect *vect, unsigned int index, nmvect *appvect);
int nmvect_contains(nmvect *vect, const void *data);
nmlist *nmvect_occurence(nmvect *vect, const void *data);
void *nmvect_get(nmvect *vect, unsigned int index);
int nmvect_set(nmvect *vect, unsigned int index, const void *data);
void *nmvect_remove(nmvect *vect, unsigned int index);
nmvect *nmvect_remove_range(nmvect *vect, unsigned int start, unsigned int stop);
int nmvect_purge(nmvect *vect, unsigned int index);
int nmvect_purge_range(nmvect *vect, unsigned int start, unsigned int stop);
unsigned int nmvect_capacity(nmvect *vect);
unsigned int nmvect_size(nmvect *size);

#endif