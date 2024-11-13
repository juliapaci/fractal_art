#include <raylib.h>

#include "shape.h"

int main(void) {
    SetTraceLogLevel(LOG_ERROR);
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Fractal Art");
    SetTargetFPS(60);

    Shape shape = shape_init(5);

    while(!WindowShouldClose()) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            shape_push(&shape, GetMousePosition());

        BeginDrawing();
            ClearBackground(DARKGRAY);
            shape_draw(&shape);

            DrawCircleLinesV(GetMousePosition(), POINT_RADIUS, POINT_COLOUR);
        EndDrawing();
    }

    CloseWindow();
    shape_free(&shape);
    return 0;
}
