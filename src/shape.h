#include <raylib.h>
#include <stdlib.h>

#define SMALL_ALLOC 5
#define BIG_ALLOC PREDICTION_DEPTH

#define PREDICTION_DEPTH 1000

#define POINT_RADIUS 5
#define POINT_COLOUR GREEN
#define LINE_COLOUR POINT_COLOUR
#define ENDING_COLOUR PURPLE

#define SHAPE_PREDICTION(shape, body) do {  \
    if(shape->used == 0)  \
        continue;  \
  \
    const Vector2 mpos = GetMousePosition();  \
  \
    const Vector2 last = shape->points[shape->used - 1];  \
    const float angle = -Vector2LineAngle(mpos, last);  \
    const Vector2 line = Vector2Scale(  \
        Vector2Normalize(Vector2Subtract(mpos, last)),  \
        Vector2Distance(mpos, last)  \
    );  \
  \
    Vector2 prev_pont = mpos;  \
    for(size_t i = 0; i < PREDICTION_DEPTH; i++) {  \
        const Vector2 point = _rotate_point(Vector2Add(prev_pont, Vector2Scale(line, (PREDICTION_DEPTH - i)*(float)1./PREDICTION_DEPTH)), prev_pont, angle*(1 + i));  \
  \
        body  \
  \
        prev_pont = point;  \
    }  \
} while(false);

// dynamic array
typedef struct {
    Vector2 *points;
    size_t capacity;
    size_t used;
} Shape;

Shape shape_init(size_t capacity);
void shape_push(Shape *shape, Vector2 point);
void shape_prediction_push(Shape *shape);
void shape_remove(Shape *shape, size_t index);
void shape_free(Shape *shape);

Vector2 _rotate_point(Vector2 point, Vector2 pivot, float angle);
void shape_draw_prediction(Shape *shape);
void shape_draw(Shape *shape);
