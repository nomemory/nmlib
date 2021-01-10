#include <stdlib.h>
#include "nmaux.h"

/** Simple frees the pointer to data.
 * If data points to a more sophisticated data structure,
 * you should build your own more advanced
 * destructor */
void nmaux_primitive_destructor(void *data)
{
	free(data);
}