#include <raylib.h>
#include <raymath.h>

#include "shape.h"

int main(void) {
    SetTraceLogLevel(LOG_ERROR);
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Fractal Art");
    SetTargetFPS(60);

    Shape shape = shape_init();

    while(!WindowShouldClose()) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            shape_push(&shape, GetMousePosition());
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            shape_prediction_push(&shape);

        BeginDrawing();
            ClearBackground(DARKGRAY);
            shape_draw(&shape);
            shape_draw_prediction(&shape, PREDICTION(shape.used - 1));

            DrawCircleLinesV(GetMousePosition(), POINT_RADIUS, POINT_COLOUR);
        EndDrawing();
    }

    CloseWindow();
    shape_free(&shape);
    return 0;
}
