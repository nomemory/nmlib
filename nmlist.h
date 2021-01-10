#ifndef __NM__LIST__H__
#define __NM__LIST__H__

typedef struct nmlist_element_s nmlist_element;
typedef struct nmlist_s nmlist;
	
nmlist *nmlist_alloc(void (*destructor)(void *data));
int nmlist_free(nmlist *list);

int nmlist_insert_next(nmlist *list, nmlist_element *element, const void *data);
int nmlist_insert_index(nmlist *list, unsigned int index, const void *data);

void *nmlist_remove_next(nmlist *list, nmlist_element *element);
void *nmlist_remove_index(nmlist *list, unsigned int index);

int nmlist_purge_next(nmlist *list, nmlist_element *element);
int nmlist_purge_index(nmlist *list, unsigned int index);

unsigned int nmlist_size(nmlist *list);
nmlist_element *nmlist_head(nmlist *list);
nmlist_element *nmlist_tail(nmlist *list);
nmlist_element *nmlist_next(nmlist_element *element);
nmlist_element *nmlist_index(nmlist *list, unsigned int index);

void *nmlist_get_data(nmlist_element *element);
void *nmlist_get_head(nmlist *list);
void *nmlist_get_tail(nmlist *list);
void *nmlist_get_next(nmlist_element *element);
void *nmlist_get_index(nmlist *list, unsigned int index);
void (*nmlist_get_destructor(nmlist *list))(void *data);

int nmlist_set_data(nmlist_element *element, const void *data);
int nmlist_set_head(nmlist *list, const void *data);
int nmlist_set_tail(nmlist *list, const void *data);
int nmlist_set_next(nmlist_element *element, const void *data);
int nmlist_set_index(nmlist *list, unsigned int index, const void *data);
int nmlist_set_destructor(nmlist *list, void(*destructor)(void *data));

#endif