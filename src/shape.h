#include <raylib.h>
#include <stdlib.h>

#define POINT_RADIUS 5
#define POINT_COLOUR GREEN
#define LINE_COLOUR POINT_COLOUR
#define ENDING_COLOUR PURPLE

#define PREDICTION_DEPTH 2

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

Vector2 _rotate_point(Vector2 point, Vector2 pivot, float angle);
void shape_draw(Shape *shape);
