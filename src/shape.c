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

    Vector3 components = {0};
    Color colour = LINE_COLOUR;
    Vector2 prev_pont = prediction.mpos;
    for(size_t i = 0; i < PREDICTION_DEPTH; i++) {
        const Vector2 point = _rotate_point(
            Vector2Add(
                prev_pont,
                Vector2Scale(line,
                    1.0 - (float)i/PREDICTION_DEPTH)
                ),
            prev_pont,
            angle*(1 + i)
        );

        const Color prev_colour = colour;
        const uint8_t field = i % 3;
        float *component = (float *)&components + field;
        *component += 3./PREDICTION_DEPTH;
        *((uint8_t *)&colour + field) += 255 * *component;

        DrawLineGradient(prev_pont, point, 1.0, prev_colour, colour);
        prev_pont = point;
    }
}

void shape_draw(Shape *shape) {
    if(shape->used == 0)
        return;

    DrawLineV(GetMousePosition(), shape->points[shape->used - 1], LINE_COLOUR);

    for(size_t i = 0; i < shape->used; i++)
        DrawCircleV(shape->points[i], POINT_RADIUS, POINT_COLOUR);

    for(size_t i = 0; i < shape->p_used; i++)
        shape_draw_prediction(shape, shape->predictions[i]);

    DrawLineStrip(shape->points, shape->used, LINE_COLOUR);
}
