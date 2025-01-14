#ifndef GAME 
#define GAME



#include "raylib.h"
#include <stdint.h>


// Collisions notes:
// https://gamedev.stackexchange.com/questions/27596/implementing-separating-axis-theorem-sat-and-minimum-translation-vector-mtv
// https://www.youtube.com/watch?v=dn0hUgsok9M

// General structure
//
// trying out _none ecs_ procedural approach
// don't want to just rawly sync stuff thought, should be extensible and somewhat efficient
//
// GameState struct
//  List of objects and other information about game
//
//
//  Things to sync
//  Game data (scoring, game state (like literally enum of is round over or whatever), etc.)
//  Player data (input, positioning, etc.)
//  AI characters
//
//
//  Give ID + size of struct + data payload
//  How to sync more complicated data structures? 
//


#define MAX_PLAYER_NUM 4
#define MAX_TEAM_NUM 2
#define MAX_ENTITIES 100

typedef struct IVector2 {
    int x;
    int y;
} IVector2; 

typedef struct GameTransform {
    Vector2 pos;
    Vector2 vel;
    Vector2 accel;
    Vector2 size;
} GameTransform;


typedef enum EGridType {
    EM = 0, // Empty
    WA = 1, // Wall

} EGridType;

typedef struct TeamMapData {
    IVector2 flag;
    IVector2 spawn;
} TeamMapData;


static EGridType default_map_data[] = { 
       WA, WA, WA, WA, WA, WA, WA, WA, WA, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, WA, WA, WA, WA, WA, WA, WA, WA, WA,
    };


typedef struct Map {
    int width;
    int height;
    EGridType* map_data;
} Map;


static Map default_map = {
    .width = 10,
    .height = 10,
    .map_data = default_map_data,
};

typedef enum __attribute__((__packed__)) EBoxType {
    DMG = 0,
    HP = 1,
} EBoxType;

typedef enum __attribute__((__packed__)) ECarrying {
    NONE_C = 0,
    FLAG_C = 1,
    BOX_C = 2,
} ECarrying;

typedef enum __attribute__((__packed__)) EClass {
    SCOUT = 0,
    HEAVY = 1,
    DEMO = 2,
} EClass;

typedef int EntityID;

typedef enum EPlayerControllerType {
    CT_NULL_CONTROLLER = 0,
    CT_LOCAL = 1,
    CT_NETWORK = 2,
    CT_AI = 3,
} EPlayerControllerType;

typedef enum EPlayerInput {
    UP =    0b000001,
    DOWN =  0b000010,
    LEFT =  0b000100,
    RIGHT = 0b001000,
    SHOOT = 0b010000,
    DASH = 0b100000,
} EPlayerInput;


typedef struct Player {
    GameTransform transform;
    EClass player_class;
    ECarrying carrying;
    EPlayerInput input;
    float x_dir;
    float y_dir;
    uint8_t dashes;
    int8_t hp;
} Player;


typedef struct PlayerController {
    EPlayerControllerType type;
    EntityID local_player;
    float aim_dir;
    EPlayerInput input;
} PlayerController;

extern PlayerController controllers[MAX_PLAYER_NUM];

typedef enum EntityType {
    NONE_ENT = 0,
    WALL = 1,
    UPGRADEBOX = 2,
    UPGRADEBOX_SPAWNER = 3,
    HEALTH_SPAWNER = 4,
    FLAG = 5,
    PLAYER = 6,
} EntityType;


typedef struct UpgradeBoxSpawner {
    GameTransform transform;
    EBoxType type;
    bool available;
} UpgradeBoxSpawner;


typedef struct HealthSpawner {
    GameTransform transform;
    bool available;
} HealthSpawner;

typedef struct Flag {
    GameTransform transform;
} Flag;

typedef struct Entity {
    EntityType type;
    EntityID id;
    void* data;
} Entity;


    //EntityID players[MAX_PLAYER_NUM];
    //int player_num;
typedef struct GameData {
    Entity entities[MAX_ENTITIES];
    int entity_num;
    int next_entity_index;
} GameData;


// Game Management Functions
GameData CreateGameData();
void UpdateEntity(Entity* ent);
EntityID CreateEntity(GameData* dat, EntityType type);
void ProcessEntity(Entity* ent);
void* GetEntityData(GameData* dat, EntityID ent);
void DrawEntity(Entity* ent);


void DrawGame(GameData* data);
void ProcessInputs(GameData* data);
void ProcessEntities(GameData* data);

// Player functions
void DrawPlayer(Player* player);

int8_t GetMaxHP(Player* player);
uint8_t GetMaxDashes(Player* player);

#endif
