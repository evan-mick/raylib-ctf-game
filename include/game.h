#include "raylib.h"
#include <stdint.h>

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

typedef struct IVector2 {
    int x;
    int y;
} IVector2; 

typedef struct GameTransform {
    Vector2 pos;
    Vector2 vel;
    Vector2 accel;
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

typedef struct Player {
    GameTransform transform;
    EClass player_class;
    float aim_dir;
    uint8_t dashes;
    int8_t hp;
} Player;

typedef struct UpgradeBox {
    GameTransform transform;
    ECarrying type;
} UpgradeBox;

typedef struct Flag {
    GameTransform transform;
    ECarrying type;
} Flag;

void DrawPlayer(Player* player);

int8_t GetMaxHP(Player* player);
uint8_t GetMaxDashes(Player* player);
