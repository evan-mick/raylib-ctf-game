#include "raylib.h"
#include "helpers.h"
#include "game.h"
#include <string.h>

#define DEBUG 1
#ifdef DEBUG
#include <stdio.h>
#define debug_print printf
#else
#define debug_print (void)
#endif
// Game flow
// Menu
// Lobby
//    Four players, picking teams, picking classes
// Game
//    Starting countdown
//    gameplay loop***
//    End, to lobby


int main() {

   const int TILE_SIZE = 32;
   const int BORDER_SIZE = TILE_SIZE *2;

   const int width = TILE_SIZE * 10 + BORDER_SIZE*2;
   const int height = TILE_SIZE * 10;

   InitWindow(width, height, "Test");
   SetTargetFPS(60);


   //Player player = {};
   //player.transform.pos = (Vector2){ width/2.f, height/2.f };

   debug_print("start");
   GameData current_data = (GameData) {
      .entities = {0},
      .entity_num = 0,
      .next_entity_index = 0
   };
   EntityID player_test_ent = CreateEntity(&current_data, PLAYER);

   memset(&controllers, 0, sizeof(PlayerController)*MAX_PLAYER_NUM);
   controllers[0].type = CT_LOCAL;
   controllers[0].local_player = player_test_ent;

   while (!WindowShouldClose()) {

      //debug_print("poll event\n");
      ProcessInputs(&current_data);
      Color col = IsKeyDown(KEY_SPACE) ? GREEN : YELLOW;
      ProcessEntities(&current_data);
      
      BeginDrawing();
      ClearBackground(RAYWHITE);
      for (int x = 0; x < default_map.width; x++) {
         for (int y = 0; y < default_map.height; y++) {
            DrawRectangle(x * TILE_SIZE + BORDER_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLACK);
            DrawRectangleLines(x * TILE_SIZE + BORDER_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, col);
         }
      }
      DrawGame(&current_data);
      EndDrawing();
   }
   CloseWindow();
   return 0;
}
