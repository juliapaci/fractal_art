#include "shape.h"

#include <raymath.h>
#include <stdint.h>

Shape shape_init(void) {
    return (Shape) {
        .point_size = sizeof(Vector2),
        .point_da = da_init(sizeof(Vector2)),

        .prediction_size = sizeof(Prediction),
        .prediction_da = da_init(sizeof(Prediction))
    };
}

Vector2 shape_point_get(Shape *shape, size_t index) {
    return *(Vector2 *)da_get(&shape->point_da, index);
}

Prediction shape_prediction_get(Shape *shape, size_t index) {
    return *(Prediction *)da_get(&shape->prediction_da, index);
}

void shape_point_push(Shape *shape, Vector2 point) {
    da_push(&shape->point_da, &point, 0);
}

void shape_prediction_push(Shape *shape) {
    const Prediction prediction = PREDICTION(shape->point_da.used - 1);
    da_push(&shape->prediction_da, (void *)&prediction, PREDICTION_DEPTH);
}

void shape_point_remove(Shape *shape, size_t index) {
    da_remove(&shape->point_da, index);
}

void shape_prediction_remove(Shape *shape, size_t index) {
    da_remove(&shape->prediction_da, index);
}

void shape_free(Shape *shape) {
    da_free(&shape->point_da);
    da_free(&shape->prediction_da);
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
    const size_t point_amount = shape->point_da.used;
    if(point_amount == 0)
        return;

    Vector2 points[PREDICTION_DEPTH + 1];
    points[0] = prediction.pos;

    const Vector2 last = shape_point_get(shape, prediction.index);
    const Vector2 line = Vector2Scale(
        Vector2Normalize(Vector2Subtract(prediction.pos, last)),
        Vector2Distance(prediction.pos, last)
    );
    float angles[point_amount];
    for(size_t i = 0; i < point_amount - 1; i++)
        angles[i] = Vector2LineAngle(shape_point_get(shape, i), shape_point_get(shape, i + 1));
    angles[point_amount - 1] = Vector2LineAngle(last, prediction.pos);

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
            angles[i%point_amount]*(1 + i)
        );

        prev_point = point;
        points[i + 1] = point;
    }

    DrawLineStripGradient(points, PREDICTION_DEPTH + 1, LINE_COLOUR, LINE_END_COLOUR);
}

void shape_draw(Shape *shape) {
    if(shape->point_da.used == 0)
        return;

    for(size_t i = 0; i < shape->point_da.used; i++)
        DrawCircleV(shape_point_get(shape, i), POINT_RADIUS, POINT_COLOUR);

    for(size_t i = 0; i < shape->prediction_da.used; i++)
        shape_draw_prediction(shape, shape_prediction_get(shape, i));

    DrawLineStrip((Vector2 *)shape->point_da.elements, shape->point_da.used, LINE_COLOUR);
}

Shapes shapes_init(void) {
    return (Shapes) {
        .da = da_init(sizeof(Shape))
    };
}

Shape *shapes_get(Shapes *shapes, size_t index) {
    return (Shape *)da_get(&shapes->da, index);
}

void shapes_push(Shapes *shapes, Shape *shape) {
    da_push(&shapes->da, shape, 0);
}

void shapes_free(Shapes *shapes) {
    for(size_t i = 0; i < shapes->da.used; i++)
        shape_free(shapes_get(shapes, i));

    da_free(&shapes->da);
}
