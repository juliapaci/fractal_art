#include <raylib.h>
#include <stdlib.h>

#define POINT_RADIUS 5
#define POINT_COLOUR GREEN
#define LINE_COLOUR POINT_COLOUR

// dynamic array
typedef struct {
    Vector2 *points;
    size_t capacity;
    size_t used;
} Shape;

Shape shape_init(size_t capacity);
void shape_push(Shape *shape, Vector2 point);
void shape_remove(Shape *shape, size_t index);
void shape_free(Shape *shape);

void draw_shape(Shape *shape);
