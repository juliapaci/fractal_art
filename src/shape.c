#include "shape.h"

#include <raymath.h>
#include <stdint.h>

Shape shape_init(size_t capacity) {
    return (Shape) {
        .points = malloc(capacity * sizeof(Vector2)),
        .capacity = capacity,
        .used = 0
    };
}

void shape_push(Shape *shape, Vector2 point) {
    if(shape->used == shape->capacity) {
        shape->capacity += SMALL_ALLOC;
        shape->points = realloc(shape->points, shape->capacity * sizeof(Vector2));
    }

    shape->points[shape->used++] = point;
}

void shape_prediction_push(Shape *shape) {
    if(shape->used + PREDICTION_DEPTH >= shape->capacity) {
        shape->capacity += BIG_ALLOC;
        shape->points = realloc(shape->points, shape->capacity * sizeof(Vector2));
    }

    SHAPE_PREDICTION(shape, {
        shape->points[shape->used++] = point;
    });
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

Vector2 _rotate_point(Vector2 point, Vector2 pivot, float angle) {
    return Vector2Add(
        Vector2Rotate(
            Vector2Subtract(point, pivot),
            angle
        ),
        pivot
    );
}

void shape_draw_prediction(Shape *shape) {
    if(shape->used == 0)
        return;

    const Vector2 mpos = GetMousePosition();

    const Vector2 last = shape->points[shape->used - 1];
    const float angle = -Vector2LineAngle(mpos, last);
    const Vector2 line = Vector2Scale(
        Vector2Normalize(Vector2Subtract(mpos, last)),
        Vector2Distance(mpos, last)
    );

    Vector2 prev_pont = mpos;
    for(size_t i = 0; i < PREDICTION_DEPTH; i++) {
        const Vector2 point = _rotate_point(Vector2Add(prev_pont, Vector2Scale(line, (PREDICTION_DEPTH - i)*(float)1./PREDICTION_DEPTH)), prev_pont, angle*(1 + i));

        DrawLineV(prev_pont, point, LINE_COLOUR);
        prev_pont = point;
    }
}

void shape_draw(Shape *shape) {
    if(shape->used == 0)
        return;

    const Vector2 mpos = GetMousePosition();

    DrawCircleV(shape->points[0], POINT_RADIUS, POINT_COLOUR);
    DrawLineV(mpos, shape->points[shape->used - 1], LINE_COLOUR);

    for(size_t i = 0; i < shape->used-1; i++) {
        const Vector2 prev = shape->points[i];
        const Vector2 next = shape->points[i + 1];

        DrawCircleLinesV(prev, POINT_RADIUS, POINT_COLOUR);
        DrawCircleLinesV(next, POINT_RADIUS, POINT_COLOUR);

        DrawLineV(prev, next, LINE_COLOUR);
    }
}
