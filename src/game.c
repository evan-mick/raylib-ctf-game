#include "game.h"
#include "helpers.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PlayerController controllers[MAX_PLAYER_NUM];

void* GetEntityData(GameData *dat, EntityID ent) {

   if (dat->entities[ent].type == NONE_ENT)
      return NULL;


   return dat->entities[ent].data;
}

void DrawGame(GameData* data) {
   int entities_processed = 0;
   int index = 0;
   while (entities_processed < data->entity_num) {
      Entity* ent = &data->entities[index];
      index++;
      if (ent->type == NONE_ENT)
         continue;
      DrawEntity(ent);
      entities_processed++;
   } 
}


void ProcessInputs(GameData* data) {
   PollInputEvents();
   //printf("PROCESSING INPUTS");
   for (int i = 0; i < MAX_PLAYER_NUM; i++) {

      printf("CONTR %d\n", controllers[i].type);
      if (controllers[i].type == CT_NULL_CONTROLLER) 
         continue;

      EntityID playerID = controllers[i].local_player;

      Player* player = (Player*)GetEntityData(data, playerID);
      if (player == NULL) {
         printf("PLAYER NULL\n");
         continue;
      }
      // TODO, get input from controllers
      //


      player->input = 
         ((IsKeyDown(KEY_LEFT_SHIFT) << 5) |
         (IsMouseButtonPressed(0) << 4) |
         (IsKeyDown(KEY_D) << 3) |
         (IsKeyDown(KEY_A) << 2) |
         (IsKeyDown(KEY_S) << 1) |
         (IsKeyDown(KEY_W)));

      bool up = ((player->input & UP) != 0);
      bool down = ((player->input & DOWN) != 0);
      bool left = ((player->input & LEFT) != 0);
      bool right = ((player->input & RIGHT) != 0);
      player->x_dir = right - left; 
      player->y_dir = down - up; 

      printf("Input %d\n", player->input);
   }

}

void ProcessEntities(GameData* data) {

   int entities_processed = 0;
   int index = 0;
   while (entities_processed < data->entity_num) {
      Entity* ent = &(data->entities[index]);
      index++;

      if (ent->type == NONE_ENT)
         continue;

      ProcessEntity(ent);
      entities_processed++;
   }

}

EntityID CreateEntity(GameData* data, EntityType type) {
   Entity entity = (Entity){};
   entity.id = data->next_entity_index;
   entity.type = type; 

   Player* player;

   switch (type) {
   case NONE_ENT:
      break;
   case WALL:
      break;
   case UPGRADEBOX:
      break;
   case UPGRADEBOX_SPAWNER:
      break;
   case HEALTH_SPAWNER:
      break;
   case FLAG:
     break;
   case PLAYER:
         player = malloc(sizeof(Player));
         memset(player, 0, sizeof(Player));
         player->transform.pos = (Vector2) { 80.f, 80.f };

         entity.data = player;
      break;
   }

   data->entities[data->next_entity_index] = entity;

   data->entity_num++;
   data->next_entity_index++;
   return entity.id;
}

void DrawEntity(Entity* ent) {
   if (ent == NULL || ent->data == NULL)
      return;

   switch (ent->type) {
   case NONE_ENT:
      break;
   case WALL:
      break;
   case UPGRADEBOX:
      break;
   case UPGRADEBOX_SPAWNER:
      break;
   case HEALTH_SPAWNER:
      break;
   case FLAG:
     break;
   case PLAYER:
         DrawPlayer((Player*)ent->data);
      break;
   }
}
void ProcessEntity(Entity* ent) {
   Player* player;
   if (ent == NULL || ent->data == NULL)
      return;

   switch (ent->type) {
   case WALL:
      break;
   case UPGRADEBOX:
      break;
   case UPGRADEBOX_SPAWNER:
      break;
   case HEALTH_SPAWNER:
      break;
   case FLAG:
     break;
   case PLAYER:

      player = (Player*)(ent->data);
      

      Vector2 mov = NormalizeVector((Vector2){ player->x_dir, player->y_dir } );
      printf("PLAYER PROCESS %d %f %f %lu\n", ent->id, mov.x, mov.y, sizeof(Player));

      player->transform.pos = (Vector2){ mov.x + player->transform.pos.x, mov.y + player->transform.pos.y }; 


      break;
   default:
      break;
   }
}

void DestroyEntity(GameData* dat, EntityID ent) {

   if (dat == NULL || dat->entities[ent].type == NONE_ENT)
      return;


   free(dat->entities[ent].data);
   dat->entities[ent].type = NONE_ENT;
   dat->entities[ent].id = 0;
   dat->entities[ent].data = NULL;

   dat->entity_num--;
   // TODO: update next entity index
   //dat->next_entity_index
}

void DamagePlayer(Player *player, int amt) {
   player->hp -= amt;

   // Death
   if (player->hp <= 0) {

   }
}


void DrawPlayer(Player *player) {
   if (player == 0)
      return;
   DrawRectangleV(player->transform.pos, (Vector2){ 24.f, 24.f }, WHITE);
}


GameData CreateGameData() {
   GameData ret = {
      .entities = {},
      .entity_num = 0,
      .next_entity_index = 0
   };


   return ret; 
}



