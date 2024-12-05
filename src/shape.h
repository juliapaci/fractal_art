#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <raylib.h>
#include <stdlib.h>
#include "da.h"

#define PREDICTION_DEPTH 1000

#define POINT_RADIUS 5
#define POINT_COLOUR GREEN
#define LINE_COLOUR POINT_COLOUR
#define LINE_END_COLOUR PURPLE

typedef struct {
    size_t index;

    // state
    size_t point_amount;
    Vector2 pos;
} Prediction;

#define PREDICTION(shape, index) (Prediction){index, (shape).point_da.used, GetMousePosition()}

// dynamic array
typedef struct {
    // points
    struct DA point_da;

    // indecies of points whose predictions will be drawn
    struct DA prediction_da;
} Shape;

Shape shape_init(void);
Vector2 shape_point_get(Shape *shape, size_t index);
Prediction shape_prediction_get(Shape *shape, size_t index);
void shape_point_push(Shape *shape, Vector2 point);
void shape_prediction_push(Shape *shape);
void shape_point_remove(Shape *shape, size_t index);
void shape_prediction_remove(Shape *shape, size_t index);
void shape_free(Shape *shape);

Vector2 _rotate_point(Vector2 point, Vector2 pivot, float angle);
void shape_draw_prediction(Shape *shape, Prediction prediction);
void shape_draw(Shape *shape);

typedef struct {
    struct DA da;
} Shapes;

Shapes shapes_init(void);
Shape *shapes_get(Shapes *shapes, size_t index);
void shapes_push(Shapes *shapes, Shape *shape);
void shapes_free(Shapes *shapes);

#endif // __SHAPE_H_
