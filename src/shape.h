#include <raylib.h>
#include <stdlib.h>

#define SMALL_ALLOC 5

#define PREDICTION_DEPTH 100

#define POINT_RADIUS 5
#define POINT_COLOUR GREEN
#define LINE_COLOUR POINT_COLOUR
#define ENDING_COLOUR PURPLE

typedef struct {
    size_t index;
    Vector2 mpos;
} Prediction;

#define PREDICTION(index) (Prediction){index, GetMousePosition()}

// dynamic array
typedef struct {
    Vector2 *points;
    size_t capacity;
    size_t used;

    // indecies of points whose predictions will be drawn
    Prediction *predictions;
    size_t p_capacity;
    size_t p_used;
} Shape;

Shape shape_init();
void shape_push(Shape *shape, Vector2 point);
void shape_prediction_push(Shape *shape);
void shape_remove(Shape *shape, size_t index);
void shape_prediction_remove(Shape *shape, size_t index);
void shape_free(Shape *shape);

Vector2 _rotate_point(Vector2 point, Vector2 pivot, float angle);
void shape_draw_prediction(Shape *shape, Prediction prediction);
void shape_draw(Shape *shape);
