#include "raylib.h"
#include "helpers.h"
#include "game.h"
#include <stdio.h>

int main() {

   const int TILE_SIZE = 32;
   const int BORDER_SIZE = TILE_SIZE *2;

   const int width = TILE_SIZE * 10 + BORDER_SIZE*2;
   const int height = TILE_SIZE * 10;

   InitWindow(width, height, "Test");
   SetTargetFPS(60);


   Player player = {};
   player.transform.pos = (Vector2){ width/2.f, height/2.f };

   while (!WindowShouldClose()) {
      BeginDrawing();
      ClearBackground(RAYWHITE);

      PollInputEvents();
      Color col = IsKeyDown(KEY_SPACE) ? GREEN : YELLOW;

      Vector2 mov = NormalizeVector((Vector2){ IsKeyDown(KEY_D) - IsKeyDown(KEY_A), IsKeyDown(KEY_S) - IsKeyDown(KEY_W) } );

      player.transform.pos = (Vector2){ mov.x + player.transform.pos.x, mov.y + player.transform.pos.y }; 

      for (int x = 0; x < default_map.width; x++) {
         for (int y = 0; y < default_map.height; y++) {
            DrawRectangle(x * TILE_SIZE + BORDER_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);
            DrawRectangleLines(x * TILE_SIZE + BORDER_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, col);
         }
      }
      DrawPlayer(&player);
      
      //DrawText("Working", 20, 20, 20, BLACK);
      EndDrawing();
   }
   CloseWindow();
   return 0;
}
