#include <raylib.h>
#include <raymath.h>

#include <stdio.h>

#include "shape.h"

int main(void) {
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Fractal Art");
    SetTargetFPS(60);

    Shapes shapes = shapes_init();
    Shape shape_default = (Shape){0};
    shapes_push(&shapes, &shape_default);

    bool info = true;
    while(!WindowShouldClose()) {
        const size_t shapes_amount = shapes.da.used;
        Shape *shape = shapes_get(&shapes, shapes_amount - 1);
        if(shapes_amount == 0)
            shape = &(Shape){0};

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            shape_point_push(shape, GetMousePosition());
        else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            shape_prediction_push(shape);
        else if(IsKeyPressed(KEY_S)) {
            if(IsKeyDown(KEY_LEFT_SHIFT))
                TakeScreenshot("fractal_drawing.png");
            else
                shapes_push(&shapes, &shape_default);
        }
        else if(IsKeyPressed(KEY_D))
            info = !info;

        BeginDrawing();
            ClearBackground(DARKGRAY);
            for(size_t i = 0; i < shapes_amount; i++)
                shape_draw(shapes_get(&shapes, i));

            if(info && shape->point_da.used > 0) {
                shape_draw_prediction(shape, PREDICTION(shape->point_da.used - 1));
                DrawLineV(GetMousePosition(), shape_point_get(shape, shape->point_da.used - 1), LINE_COLOUR);
            }

            DrawCircleLinesV(GetMousePosition(), POINT_RADIUS, POINT_COLOUR);
        EndDrawing();
    }

    CloseWindow();
    shapes_free(&shapes);
    return 0;
}
