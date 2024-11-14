#include "shape.h"

#include <raymath.h>
#include <stdint.h>

Shape shape_init() {
    return (Shape) {
        .points = malloc(SMALL_ALLOC * sizeof(Vector2)),
        .capacity = SMALL_ALLOC,
        .used = 0,

        .predictions = malloc(SMALL_ALLOC * sizeof(Prediction)),
        .p_capacity = SMALL_ALLOC,
        .p_used = 0
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
    if(shape->p_used + PREDICTION_DEPTH >= shape->p_capacity) {
        shape->p_capacity += PREDICTION_DEPTH;
        shape->predictions = realloc(shape->predictions, shape->p_capacity * sizeof(Prediction));
    }

    shape->predictions[shape->p_used++] = PREDICTION(shape->used - 1);
}

void shape_remove(Shape *shape, size_t index) {
    shape->used--;
    for(size_t i = index; i < shape->used; i++)
        shape->points[i] = shape->points[i + 1];
}

void shape_prediction_remove(Shape *shape, size_t index) {
    shape->p_used--;
    for(size_t i = index; i < shape->p_used; i++)
        shape->predictions[i] = shape->predictions[i + 1];
}

void shape_free(Shape *shape) {
    free(shape->points);
    shape->points = NULL;
    shape->used = 0;
    shape->capacity = 0;

    free(shape->predictions);
    shape->predictions = NULL;
    shape->p_used = 0;
    shape->p_capacity = 0;
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

void shape_draw_prediction(Shape *shape, Prediction prediction) {
    if(shape->used == 0)
        return;

    const Vector2 last = shape->points[prediction.index];
    const float angle = -Vector2LineAngle(prediction.mpos, last);
    const Vector2 line = Vector2Scale(
        Vector2Normalize(Vector2Subtract(prediction.mpos, last)),
        Vector2Distance(prediction.mpos, last)
    );

    Vector2 prev_pont = prediction.mpos;
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

    size_t prediction = 0;
    for(size_t i = 0; i < shape->used-1; i++) {
        const Vector2 prev = shape->points[i];
        const Vector2 next = shape->points[i + 1];

        DrawCircleLinesV(prev, POINT_RADIUS, POINT_COLOUR);
        DrawCircleLinesV(next, POINT_RADIUS, POINT_COLOUR);

        DrawLineV(prev, next, LINE_COLOUR);

        if(i < shape->p_used - 1 && i == shape->predictions[prediction].index) {
            DrawText(TextFormat("%d", prediction), 20, 20, 30, BLUE);
            prediction++;
            shape_draw_prediction(shape, shape->predictions[prediction]);
        }
    }

    if(shape->p_used >= 1 && shape->predictions[shape->p_used - 1].index == shape->used - 1)
        shape_draw_prediction(shape, shape->predictions[shape->p_used - 1]);
}
