#include "shape.h"

Shape shape_init(size_t capacity) {
    return (Shape) {
        .points = malloc(capacity * sizeof(Vector2)),
        .capacity = capacity,
        .used = 0
    };
}

void shape_push(Shape *shape, Vector2 point) {
    if(shape->used == shape->capacity) {
        shape->capacity += 5;
        shape->points = realloc(shape->points, shape->capacity * sizeof(Vector2));
    }

    shape->points[shape->used++] = point;
}

void shape_remove(Shape *shape, size_t index) {
    shape->used--;
    for(size_t i = index; i < shape->used; i++)
        shape->points[i] = shape->points[i + 1];
}

void shape_free(Shape *shape) {
    free(shape->points);
    shape->points = NULL;
    shape->used = 0;
    shape->capacity = 0;
}

void draw_shape(Shape *shape) {
    if(shape->used > 0)
        DrawCircleV(shape->points[0], POINT_RADIUS, POINT_COLOUR);

    for(ssize_t i = 0; i < (ssize_t)shape->used-1; i++) {
        const Vector2 prev = shape->points[i];
        const Vector2 next = shape->points[i + 1];

        DrawCircleLinesV(prev, POINT_RADIUS, POINT_COLOUR);
        DrawCircleLinesV(next, POINT_RADIUS, POINT_COLOUR);

        DrawLineV(prev, next, LINE_COLOUR);
    }
}
