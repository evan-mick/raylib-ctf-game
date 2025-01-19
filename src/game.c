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



GameTransform* GetEntityTransform(GameData* dat, EntityID ent) {
   if (dat == NULL || dat->entities[ent].type == NONE_ENT)
      return NULL;
   return &(dat->entities[ent].transform);
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
      GameTransform* player_transform = (GameTransform*)&(data->entities[playerID].transform); 
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
      player->aim_dir = atan2f(mouse_y - player_transform->pos.y, mouse_x - player_transform->pos.x);
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
      } else if (ent->type == WALL) {

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
         entity.transform.pos = (Vector2) { 80.f, 80.f };
         entity.transform.size = GetPlayerSize(CLASS_NONE);
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
   if (ent == NULL) {
      printf("NULL DRAW");
      return;
   }

   switch (ent->type) {
   case NONE_ENT:
      break;
   case WALL:
         DrawRectangleV(ent->transform.pos, ent->transform.size, YELLOW);
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
         DrawPlayer(ent);
      break;
   case E_BULLET:
         DrawRectangleV(ent->transform.pos, (Vector2){ 20.f, 20.f }, YELLOW);
     break;
   }
}
void ProcessEntity(GameData* data, Entity* ent) {
   if (ent == NULL) {
      //printf("null ent %d\n", ent->id);
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
      
      if (GetTimerVal(&(data->timer_manager), ent->id, 0) > 0.f) 
            return;

      // Timer done, and we were dead, so respawn
      if (player->hp <= 0) {
         // TODO: make dynamic
         player->hp = 3;
            
         ent->transform.pos =  (Vector2) { 80.f, 80.f };
      }
      //ProcessGameTransform(&(player->transform));

      Vector2 mov = NormalizeVector((Vector2){ player->x_dir, player->y_dir } );
      //printf("PLAYER PROCESS %d %f %f %lu\n", ent->id, mov.x, mov.y, sizeof(Player));
      

      ent->transform.pos = (Vector2){ 100.f*mov.x*GetFrameTime() + ent->transform.pos.x, 100.f*GetFrameTime()*mov.y + ent->transform.pos.y }; 

      //Vector2 offset = CheckCollisionsPhysical(data, ent->id, CL_WALL, (mov.x > mov.y));

     // if (offset.x > 0.f || offset.y > 0.f)
     //    printf("OFFSET %f %f \n", offset.x, offset.y);
     // ent->transform.pos.x += offset.x;
     // ent->transform.pos.y += offset.y;

      PlayerTriggerCheck(data, ent);

      bool shoot = ((player->input & SHOOT) != 0);

      if (shoot && GetTimerVal(&(data->timer_manager), ent->id, 1) <= 0.f) {
            EntityID bullet_ent = CreateEntity(data, E_BULLET);
            if (bullet_ent == -1)
               break;
            printf("bullet shot %d\n", bullet_ent);
            Bullet* bull = (Bullet*)GetEntityData(data, bullet_ent);
            if (bull != NULL) {
               bull->owner = ent->id;

               GameTransform* bull_trans = GetEntityTransform(data, bullet_ent);

               bull_trans->size = (Vector2) { 1.f, 1.f };
               bull_trans->pos = ent->transform.pos;
               bull_trans->vel = (Vector2) { 80.0f * cos(player->aim_dir), 80.0f * sin(player->aim_dir) };
               SetTimer(&(data->timer_manager), ent->id, 1, .2f);

               //printf(" bullet pos %f %f\n", bull->transform.pos.x, bull->transform.pos.y);
            }
         }



      break;
   case E_BULLET:
         ProcessGameTransform(GetEntityTransform(data, ent->id));
         float val = GetTimerVal(&(data->timer_manager), ent->id, 0);
         if (val <= 0.f)
            QueueDestroyEntity(data, ent->id);
      break;
   default:
      break;
   }
}


Vector2 GetEntityTypeSize(EntityType type) {
   switch (type) {
      default:
         return (Vector2) { 20, 20 };
   }
}

void PlayerDropCarrying(GameData* data, Entity* player_ent) {
   GameTransform* player_transform = GetEntityTransform(data, player_ent->id);
   Player* player = (Player*)GetEntityData(data, player_ent->id);
   EntityID ent = CreateEntity(data, player->carrying);

   GameTransform* trans = GetEntityTransform(data, ent);
   *trans = *player_transform;
   trans->size = GetEntityTypeSize(data->entities[ent].type);
   player->carrying = NONE_ENT;
}

void PlayerTriggerCheck(GameData* data, Entity* player_ent) {
   GameTransform* check_trans = GetEntityTransform(data, player_ent->id);
   Player* player = (Player*)GetEntityData(data, player_ent->id);
   int entities_processed = 0;
   int index = 0;


   //printf("draw game %f", GetTime());
   while (entities_processed < data->entity_num && index < MAX_ENTITIES) {
      index++;
      if (data->entities[index].type != FLAG && data->entities[index].type != UPGRADEBOX && data->entities[index].type != E_BULLET)
         continue;
      GameTransform* cur_trans = GetEntityTransform(data, index);

      bool coll = TestCollision(check_trans->pos.x, check_trans->pos.y, check_trans->size.x, check_trans->size.y, cur_trans->pos.x, cur_trans->pos.y, cur_trans->size.x, cur_trans->size.y);

      switch (data->entities[index].type) {
         case FLAG:
            PlayerDropCarrying(data, player_ent);
            QueueDestroyEntity(data, index);
            player->carrying = FLAG; 
            break;
         case UPGRADEBOX:
            if (player->carrying != FLAG) {
               PlayerDropCarrying(data, player_ent);
               QueueDestroyEntity(data, index);
               player->carrying = UPGRADEBOX; 
            }
            break;
         case E_BULLET:
            if (((Bullet*)GetEntityData(data, index))->owner != player_ent->id) {
               QueueDestroyEntity(data, index);
               DamagePlayer(data, player_ent->id, 1);
            }
         break;
         default:
         
         break;
      }

      entities_processed++;
   }
}

void SpawnEntitiesFromMapData(GameData* data, Map* map, float tile_size) {
   for (int x = 0; x < map->width; x++) {
      for (int y = 0; y < map->height; y++) {
         int ind = y * map->width + x;

         if (map->map_data[ind] == WA) {
            EntityID wall_id = CreateEntity(data, WALL);
            GameTransform* trans = GetEntityTransform(data, wall_id);
            trans->pos.x = x*tile_size;
            trans->pos.y = y*tile_size;
            trans->size.x = tile_size;
            trans->size.y = tile_size;
         }

      }
   }

}


// This is inefficient
// I am understanding more the benefits of ECS!
Vector2 CheckCollisionsPhysical(GameData* data, EntityID checking, CollisionMask mask, bool x_mov) {
   GameTransform* check_trans = GetEntityTransform(data, checking);
   int entities_processed = 0;
   int index = 0;
   Vector2 offset = {};


   //printf("draw game %f", GetTime());
   while (entities_processed < data->entity_num && index < MAX_ENTITIES) {
      index++;
      //if ((data->collision_masks[index] & mask) == 0)
      //   continue;
      if (data->entities[index].type != WALL)
         continue;
      GameTransform* cur_trans = GetEntityTransform(data, index);

      bool coll = TestCollision(check_trans->pos.x, check_trans->pos.y, check_trans->size.x, check_trans->size.y, cur_trans->pos.x, cur_trans->pos.y, cur_trans->size.x, cur_trans->size.y);

      //printf("coll %d %f %f %f %f %f %f %f %f\n", coll, check_trans->pos.x, check_trans->pos.y, check_trans->size.x, check_trans->size.y, cur_trans->pos.x, cur_trans->pos.y, cur_trans->size.x, cur_trans->size.y);

      Vector2 cur_coll = (Vector2) {0.f , 0.f };

      if (coll) {
         cur_coll = MDTCollision(check_trans->pos.x, check_trans->pos.y, check_trans->size.x, check_trans->size.y, cur_trans->pos.x, cur_trans->pos.y, cur_trans->size.x, cur_trans->size.y, x_mov);

         printf("coll %d %f %f %f %f %f %f %f %f %f %f\n", coll, cur_coll.x, cur_coll.y, check_trans->pos.x, check_trans->pos.y, check_trans->size.x, check_trans->size.y, cur_trans->pos.x, cur_trans->pos.y, cur_trans->size.x, cur_trans->size.y);
      }

      offset = (Vector2) { offset.x + cur_coll.x, offset.y + cur_coll.y };
      entities_processed++;
   }

   return offset;
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

void DamagePlayer(GameData* data, EntityID ent, int amt) {

   Player* player = GetEntityData(data, ent);
   player->hp -= amt;

   // Death
   if (player->hp <= 0) {

      SetTimer(&(data->timer_manager), ent, 0, 5.0f);
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

void DrawPlayer(Entity* player) {
   if (player == NULL)
      return;


   Player* player_dat = player->data;

   if (player_dat->hp == 0)
      return;

   Color col;
   Vector2 size = GetPlayerSize(player_dat->player_class);

   switch (player_dat->team) {
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
      .timer_manager = { -1.f },

      .entity_num = 0,
      .next_entity_index = 0
   };
   ret.queue_destroy = CreateDynamicArray(0, sizeof(EntityID));
   return ret; 
}



void DestroyGameData(GameData* data) {
   if (data == NULL) 
      return; 

   for (int i = 0; i < MAX_ENTITIES; i++) {
      if (data->entities[i].type == NONE_ENT)
         continue; 
      free(data->entities[i].data);
   }
   DestroyDynArray(&(data->queue_destroy));
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


bool TestCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2) {
   bool in_x = ((x1 + width1) > x2) && (x1 < (x2 + width2));
   bool in_y = ((y1 + height1) > y2) && (y1 < (y2 + height2));
   return (in_x && in_y);
}

bool TestCollisionRect(Rectangle first, Rectangle second) {
//   bool in_x = (first.x + first.width > second.x) || (first.x < second.x + second.width);
//   bool in_y = (first.y + first.height > second.y) || (first.y < second.y + second.height);
   return TestCollision(first.x, first.y, first.width, first.height, second.x, second.y, second.width, second.height);
}


float min(float a, float b) {
   return a > b ? b : a;
}

float max(float a, float b) {
   return a < b ? b : a;
}

Vector2 MDTCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2, bool is_x) {

   float ent1_minx = x1;
   float ent1_maxx = x1 + width1;
   float ent1_miny = y1;
   float ent1_maxy = y1 + height1;
 
   float ent2_minx = x2;
   float ent2_maxx = x2 + width2;
   float ent2_miny = y2;
   float ent2_maxy = y2 + height2;
     
//   float in_x_right = ((x1 + width1) - x2) ;
//   float in_x_left = (x1 - (x2 + width2));
//   float in_y_down = ((y1 + height1) - y2);
//   float in_y_up = (y1 - (y2 + height2));
   //

   float x_off = 0.f;  
   float y_off = 0.f;  

   if (ent1_maxx < ent2_maxx) {
      x_off = -(ent1_maxx - ent2_minx);
   } else {
      x_off = ent2_maxx - ent1_minx;
   }

   if (ent1_maxy < ent2_maxy) {
      y_off = -(ent1_maxy - ent2_miny);
   } else {
      y_off = -(ent2_maxy - ent1_miny);
   }

   if (is_x) {
      return (Vector2) { x_off, 0.f  };
   }
   return (Vector2) { 0.f, y_off  };



   //printf("coll test %f %f %f %f\n", in_x_right, in_x_left, in_y_up, in_y_down);

   //if (((in_x_left < 0 || in_x_right < 0) && (in_y_up < 0 || in_y_down < 0)))
   //   return ret_vec;


}

Vector2 MDTCollisionRect(Rectangle first, Rectangle second, bool is_x) {
   return MDTCollision(first.x, first.y, first.width, first.height, second.x, second.y, second.width, second.height, is_x);
}

