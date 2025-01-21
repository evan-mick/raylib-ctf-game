#ifndef GAME 
#define GAME



#include "helpers.h"
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
#define MAX_ENTITIES 100 // actually max entities + 1, for some reason...
#define TIMERS_PER_ENTITY 4

typedef int32_t EntityID;
typedef uint8_t CollisionMask;
//typedef uint8_t EntityCollisionMask;

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
    RF = 2, // Red Flag
    RS = 3, // Red spawn
    BF = 4, // Blue Flag
    BS = 5, // Blue spawn

} EGridType;

typedef struct TeamMapData {
    IVector2 flag;
    IVector2 spawn;
} TeamMapData;


static EGridType default_map_data[] = { 
       WA, WA, WA, WA, WA, WA, WA, WA, WA, WA,
       WA, EM, EM, EM, EM, EM, RS, EM, RF, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, BS, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, EM, EM, WA,
       WA, EM, EM, EM, EM, EM, EM, BF, EM, WA,
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

/*typedef enum __attribute__((__packed__)) ECarrying {
    NONE_C = 0,
    FLAG_C = 1,
    BOX_C = 2,
} ECarrying;*/

typedef enum __attribute__((__packed__)) EClass {
    CLASS_NONE = 0,
    SCOUT = 1,
    HEAVY = 2,
    DEMO = 3,
} EClass;

typedef enum __attribute__((__packed__)) ETeam {
    T_NONE = 0,
    T_RED = 1,
    T_BLUE = 2,
} ETeam;

typedef enum EntityType {
    NONE_ENT = 0,
    WALL = 1,
    UPGRADEBOX = 2,
    UPGRADEBOX_SPAWNER = 3,
    HEALTH_SPAWNER = 4,
    FLAG = 5,
    PLAYER = 6,
    E_BULLET = 7,
    FLAG_SPAWN = 8,
    E_PLAYER_SPAWN = 9,
} EntityType;



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
    EClass player_class;
    EntityType carrying;
    EPlayerInput input;
    float x_dir;
    float y_dir;
    float aim_dir;
    ETeam team;
    uint8_t dashes;
    int8_t hp;
} Player;

typedef struct Bullet {
    EntityID owner;
} Bullet;

typedef struct PlayerController {
    EPlayerControllerType type;
    EntityID local_player;
    EPlayerInput input;
} PlayerController;

extern PlayerController controllers[MAX_PLAYER_NUM];

typedef struct UpgradeBoxSpawner {
    EBoxType type;
    bool available;
} UpgradeBoxSpawner;

typedef struct Wall {
} Wall;

typedef struct HealthSpawner {
    bool available;
} HealthSpawner;

typedef struct Flag {
} Flag;

typedef struct Entity {
    EntityType type;
    EntityID id;
    GameTransform transform;
    void* data;
} Entity;



typedef enum CollisionLayer {
    CL_NONE = 0, 
    CL_PLAYER = 1,
    CL_BULLET = 2, 
    CL_WALL = 4, 
} CollisionLayer;

// Time manager interface
// Add timer, entity id, timer index
// Get timer, entity id, timer index, return float of time left
//
// Ways of doing it
//  Dictionary, make key out of id and index
//  List of vectors

typedef struct Timer {
    float time_left;
} Timer;
typedef struct TimerManager {
    // map entity id to LL of timers
    // Array of fixed
    // Kind of space inefficient, too bad!
    Timer timers[MAX_ENTITIES * TIMERS_PER_ENTITY];
} TimerManager;



//EntityID players[MAX_PLAYER_NUM];
//int player_num;
typedef struct GameData {
    Entity entities[MAX_ENTITIES];
    CollisionMask collision_masks[MAX_ENTITIES];
    TimerManager timer_manager;
    int entity_num;
    int next_entity_index;
    // Is this the right spot for this?
    DynArray queue_destroy;
} GameData;


//void(*)(EntityID, EntityID) ptr; 



// Game Management Functions
GameData CreateGameData();
void DestroyGameData(GameData* data);
void UpdateEntity(Entity* ent);


EntityID CreateEmptyEntity(GameData* data);
void SetEntityType(GameData* data, EntityID id, EntityType type);
EntityID CreateEntity(GameData* dat, EntityType type);
void ProcessEntity(GameData* data, Entity* ent);
void* GetEntityData(GameData* dat, EntityID ent);
GameTransform* GetEntityTransform(GameData* dat, EntityID ent);
void DrawEntity(Entity* ent);

void QueueDestroyEntity(GameData* dat, EntityID ent);
void DestroyEntity(GameData* dat, EntityID ent);

// Timer Manager
void SetTimer(TimerManager* man, EntityID id, int timer_id, float time);
float GetTimerVal(TimerManager* man, EntityID id, int timer_id);
void ProcessTimers(GameData* man);


void ProcessGameTransform(GameTransform* transform);


void DrawGame(GameData* data);
void ProcessInputs(GameData* data);
void ProcessEntities(GameData* data);

// Player functions
void DrawPlayer(Entity* player);
Vector2 GetPlayerSize(EClass class);


void DamagePlayer(GameData* data, EntityID ent, int amt);
int8_t GetMaxHP(Player* player);
uint8_t GetMaxDashes(Player* player);
void PlayerTriggerCheck(GameData* data, Entity* player);
void PlayerDropCarrying(GameData* data, Entity* player);


bool TestCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
bool TestCollisionRect(Rectangle first, Rectangle second);
Vector2 MDTCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2, bool is_x);
Vector2 MDTCollisionRect(Rectangle first, Rectangle second, bool is_x);

Vector2 CheckCollisionsPhysical(GameData* data, EntityID checking, CollisionMask mask, bool x_mov);
void CheckCollisionsTrigger(float x1, float y1, float width, float height, EntityID checking, CollisionMask mask);

void SpawnEntitiesFromMapData(GameData* data, Map* map, float tile_size);


Vector2 GetEntityTypeSize(EntityType type);

#endif

