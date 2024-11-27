#include "shape.h"

#include <raymath.h>
#include <stdint.h>

Shape shape_init(void) {
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

#include <stdio.h>
void shape_draw_prediction(Shape *shape, Prediction prediction) {
    if(shape->used == 0)
        return;

    Vector2 points[PREDICTION_DEPTH + 1];
    points[0] = prediction.pos;

    const Vector2 last = shape->points[prediction.index];
    const float angle = -Vector2LineAngle(prediction.pos, last);
    const Vector2 line = Vector2Scale(
        Vector2Normalize(Vector2Subtract(prediction.pos, last)),
        Vector2Distance(prediction.pos, last)
    );

    Vector2 prev_point = prediction.pos;
    for(size_t i = 0; i < PREDICTION_DEPTH; i++) {
        const Vector2 point = _rotate_point(
            Vector2Add(
                prev_point,
                Vector2Scale(line,
                    1.0 - (float)i/PREDICTION_DEPTH
                )
            ),
            prev_point,
                angle*(1 + i)
        );

        prev_point = point;
        points[i + 1] = point;
    }

    DrawLineStripGradient(points, PREDICTION_DEPTH + 1, LINE_COLOUR, LINE_END_COLOUR);
}

void shape_draw(Shape *shape) {
    if(shape->used == 0)
        return;

    for(size_t i = 0; i < shape->used; i++)
        DrawCircleV(shape->points[i], POINT_RADIUS, POINT_COLOUR);

    for(size_t i = 0; i < shape->p_used; i++)
        shape_draw_prediction(shape, shape->predictions[i]);

    DrawLineStrip(shape->points, shape->used, LINE_COLOUR);
}

Shapes shapes_init(void) {
    return (Shapes) {
        .shapes = malloc(SMALL_ALLOC * sizeof(Shape)),
        .capacity = SMALL_ALLOC,
        .used = 0
    };
}

void shapes_push(Shapes *shapes, Shape *shape) {
    if(shapes->used == shapes->capacity) {
        shapes->capacity += SMALL_ALLOC;
        shapes->shapes = realloc(shapes->shapes, shapes->capacity * sizeof(shapes->capacity));
    }

    shapes->shapes[shapes->used++] = *shape;
}

void shapes_free(Shapes *shapes) {
    for(size_t i = 0; i < shapes->used; i++)
        shape_free(&shapes->shapes[i]);

    free(shapes->shapes);
    shapes->used = shapes->capacity = 0;
    shapes->shapes = NULL;
}
