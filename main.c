#include "include/raylib.h"

int main() {
   InitWindow(800, 600, "Test");
   SetTargetFPS(60);

   while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Working", 20, 20, 20, BLACK);
      EndDrawing();
   }
   CloseWindow();
   return 0;
}
