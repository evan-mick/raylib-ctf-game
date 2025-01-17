#include "game.h"
#include "helpers.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PlayerController controllers[MAX_PLAYER_NUM];

void* GetEntityData(GameData *dat, EntityID ent) {
   if (ent == -1 || ent >= MAX_ENTITIES)
      return NULL;

   if (dat->entities[ent].type == NONE_ENT)
      return NULL;

   return dat->entities[ent].data;
}

void DrawGame(GameData* data) {
   int entities_processed = 0;
   int index = 0;
   //printf("draw game %f", GetTime());
   while (entities_processed < data->entity_num && index < MAX_ENTITIES) {
      Entity* ent = &(data->entities[index]);
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
         (IsKeyDown(KEY_Z) << 4) |
         (IsKeyDown(KEY_D) << 3) |
         (IsKeyDown(KEY_A) << 2) |
         (IsKeyDown(KEY_S) << 1) |
         (IsKeyDown(KEY_W)));
      //printf("Input %d\n", player->input);

      bool up = ((player->input & UP) != 0);
      bool down = ((player->input & DOWN) != 0);
      bool left = ((player->input & LEFT) != 0);
      bool right = ((player->input & RIGHT) != 0);
      player->x_dir = right - left; 
      player->y_dir = down - up; 

      float mouse_x = GetMouseX();
      float mouse_y = GetMouseY();
      player->aim_dir = atan2f(mouse_y - player->transform.pos.y, mouse_x - player->transform.pos.x);
      //printf("aim dir %f", player->aim_dir);
   }

}

void ProcessEntities(GameData* data) {

   int entities_processed = 0;
   int index = 0;
   int init_ent_num = data->entity_num;
   while (entities_processed < init_ent_num && index < MAX_ENTITIES) {
      Entity* ent = &(data->entities[index]);
      index++;

      if (ent->type == NONE_ENT) {
         //printf("none ent process");
         continue;
      }
      //printf("process ind %d\n", index);

      ProcessEntity(data, ent);
      entities_processed++;
   }

}

EntityID CreateEntity(GameData* data, EntityType type) {

   if (data->entity_num >= MAX_ENTITIES || data->next_entity_index >= MAX_ENTITIES)
      return -1;


   Entity entity = (Entity){};
   Player* player;

   void* data_ptr = NULL;
   entity.id = data->next_entity_index;
   entity.type = type; 

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
         data_ptr = malloc(sizeof(Player));
         memset(data_ptr, 0, sizeof(Player));
         player = data_ptr;
         player->transform.pos = (Vector2) { 80.f, 80.f };
         player->transform.size = GetPlayerSize(CLASS_NONE);
         entity.data = player;
      break;
   case E_BULLET:
         printf("BULLET MADE");
         data_ptr = malloc(sizeof(Bullet));
         memset(data_ptr, 0, sizeof(Bullet));
         SetTimer(&(data->timer_manager), entity.id, 0, 4.0f);
     break;
   }
   entity.data = data_ptr;
   data->entities[data->next_entity_index] = entity;
   data->entity_num++;

   bool set = false;
   for (int i = entity.id; i < MAX_ENTITIES; i++) {
      if (data->entities[i].type == NONE_ENT) {
         data->next_entity_index = i;
         set = true;
         break;
      }
   }
   if (!set)
      data->next_entity_index = MAX_ENTITIES;
   printf("ent num %d next ent %d\n", data->entity_num, data->next_entity_index);
   return entity.id;
}

void DrawEntity(Entity* ent) {
   if (ent == NULL || ent->data == NULL) {
      printf("NULL DRAW");
      return;
   }

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
   case E_BULLET:
         DrawRectangleV(((Bullet*)(ent->data))->transform.pos, (Vector2){ 20.f, 20.f }, YELLOW);
     break;
   }
}
void ProcessEntity(GameData* data, Entity* ent) {
   if (ent == NULL || ent->data == NULL) {
      printf("null ent %d\n", ent->id);
      return;
   }

   switch (ent->type) {
   Player* player;
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
      
      //ProcessGameTransform(&(player->transform));

      Vector2 mov = NormalizeVector((Vector2){ player->x_dir, player->y_dir } );
      //printf("PLAYER PROCESS %d %f %f %lu\n", ent->id, mov.x, mov.y, sizeof(Player));
      

      player->transform.pos = (Vector2){ 100.f*mov.x*GetFrameTime() + player->transform.pos.x, 100.f*GetFrameTime()*mov.y + player->transform.pos.y }; 


      bool shoot = ((player->input & SHOOT) != 0);

      if (shoot) {
            EntityID bullet_ent = CreateEntity(data, E_BULLET);
            if (bullet_ent == -1)
               break;
            printf("bullet shot %d\n", bullet_ent);
            Bullet* bull = (Bullet*)GetEntityData(data, bullet_ent);
            if (bull != NULL) {
               bull->owner = ent->id;

               bull->transform.size = (Vector2) { 1.f, 1.f };
               bull->transform.pos = player->transform.pos;
               bull->transform.vel = (Vector2) { 80.0f * cos(player->aim_dir), 80.0f * sin(player->aim_dir) };

               //printf(" bullet pos %f %f\n", bull->transform.pos.x, bull->transform.pos.y);
            }
         }


      break;
   case E_BULLET:
         ProcessGameTransform(&(((Bullet*)GetEntityData(data, ent->id))->transform));
         float val = GetTimerVal(&(data->timer_manager), ent->id, 0);
         if (val <= 0.f)
            QueueDestroyEntity(data, ent->id);
      break;
   default:
      break;
   }
}

void QueueDestroyEntity(GameData* dat, EntityID ent) { 
   AddToDynamicArray(&(dat->queue_destroy), &ent);
}

void DestroyEntity(GameData* dat, EntityID ent) {

   if (dat == NULL || dat->entities[ent].type == NONE_ENT)
      return;


   free(dat->entities[ent].data);
   dat->entities[ent].type = NONE_ENT;
   dat->entities[ent].data = NULL;

   dat->entity_num--;
   // TODO: update next entity index
   if (ent < dat->next_entity_index) {
      dat->next_entity_index = ent;
   }
}

void DamagePlayer(Player *player, int amt) {
   player->hp -= amt;

   // Death
   if (player->hp <= 0) {

   }
}

Vector2 GetPlayerSize(EClass class) {
   float size;
   switch (class) {
   case SCOUT:
      size = 30.f;
      break;
   case HEAVY:
      size = 60.f;
      break;
   case DEMO:
      size = 40.f;
     break;
   default:
      size = 30.f;
      break;
   }

   return (Vector2){ size, size };
}

void DrawPlayer(Player *player) {
   if (player == 0)
      return;


   Color col;
   Vector2 size = GetPlayerSize(player->player_class);

   switch (player->team) {
   case T_RED:
         col = RED;
     break;
   case T_BLUE:
         col = BLUE;
      break;
   default:
         col = WHITE;
      break;
   }

   DrawRectangleV(player->transform.pos, size, col);
}

void ProcessGameTransform(GameTransform* transform) {
   if (transform == NULL)
      return;
   //printf("frame time %f\n", GetFrameTime());
   //printf(" bullet pos %f %f %f %f %f\n", transform->pos.x, transform->pos.y, GetFrameTime(), transform->vel.x, transform->vel.y);
   transform->pos = (Vector2) { transform->pos.x + (transform->vel.x * GetFrameTime()),
                              transform->pos.y + (transform->vel.y * GetFrameTime())};
   transform->vel = (Vector2) { transform->vel.x + (transform->accel.x * GetFrameTime()),
                              transform->vel.y + (transform->accel.y * GetFrameTime()) };
}

GameData CreateGameData() {
   GameData ret = {
      .entities = {0},
      .timer_manager = {-1.f},

      .entity_num = 0,
      .next_entity_index = 0
   };
   ret.queue_destroy = CreateDynamicArray(0, sizeof(EntityID));
   return ret; 
}

void SetTimer(TimerManager* man, EntityID id, int timer_id, float time) {
   if (timer_id >= TIMERS_PER_ENTITY)
      return;
   man->timers[id * TIMERS_PER_ENTITY + timer_id].time_left = time;
}

float GetTimerVal(TimerManager* man, EntityID id, int timer_id) {
   return man->timers[id * TIMERS_PER_ENTITY + timer_id].time_left;
}


void ProcessTimers(GameData* data) {
   TimerManager* man = &(data->timer_manager);

   int entities_processed = 0;
   int index = 0;
   int init_ent_num = data->entity_num;
   //while (entities_processed < init_ent_num && index < MAX_ENTITIES) {
   for (int i = 0; i < TIMERS_PER_ENTITY*MAX_ENTITIES; i++) {
     // Entity* ent = &(data->entities[index]);

      /*if (ent->type == NONE_ENT) {
         continue;
      }*/

      int time_ind = (index*TIMERS_PER_ENTITY) + i;
      float time_left = man->timers[i].time_left;
      if (time_left > 0.f) {
         float remaining_time = time_left - GetFrameTime();
         man->timers[i].time_left =
            remaining_time <= 0.f ? -1.f : remaining_time;
      }
   }




}




