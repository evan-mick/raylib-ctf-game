#include "game.h"
#include "raylib.h"


void DrawPlayer(Player *player) {
   if (player == 0)
      return;
   DrawRectangleV(player->transform.pos, (Vector2){ 24.f, 24.f }, WHITE);
}
