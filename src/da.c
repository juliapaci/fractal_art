#include "da.h"

#include <string.h>
#include <stdint.h>

struct DA da_init(size_t elem_size) {
    return (struct DA){
        .elements = malloc(SMALL_ALLOC * elem_size),
        .elem_size = elem_size,
        .used = 0,
        .capacity = SMALL_ALLOC
    };
}

void *da_get(struct DA *da, size_t index) {
    return ((uint8_t *)da->elements) + index * da->elem_size;
}

void da_push(struct DA *da, void *elem, size_t overstep) {
    if(da->used + overstep == da->capacity) {
        da->capacity += overstep == 0 ? SMALL_ALLOC : overstep;
        da->elements = realloc(da->elements, da->capacity * da->elem_size);
    }

    memcpy(
        ((uint8_t *)da->elements) + da->elem_size * da->used++,
        (uint8_t *)elem,
        da->elem_size
    );
}

void da_remove(struct DA *da, size_t index) {
    memcpy(
        ((uint8_t *)da->elements) + index * da->elem_size,
        ((uint8_t *)da->elements) + (index + 1) * da->elem_size,
        (da->used-- - index) * da->elem_size
    );
}

void da_free(struct DA *da) {
    free(da->elements);
    da->used = da->capacity = 0;
    da->elements = NULL;
}
