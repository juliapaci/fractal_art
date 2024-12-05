#ifndef __DA_H__
#define __DA_H__

#include <stdlib.h>

#define SMALL_ALLOC 5

// generalised dynamic array without meta programming
struct DA {
    void *elements;
    const size_t elem_size; // in bytes
    size_t used;
    size_t capacity;
};

struct DA da_init(size_t elem_size);
void *da_get(struct DA *da, size_t index);
void da_push(struct DA *da, void *elem, size_t overstep);
void da_remove(struct DA *da, size_t index);
void da_free(struct DA *da);

#endif // __DA_H__
