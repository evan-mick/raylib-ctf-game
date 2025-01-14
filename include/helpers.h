#ifndef HELPERS 
#define HELPERS

#include "raylib.h"
#include <math.h>

#define NULL 0

static Vector2 NormalizeVector(Vector2 vec) {
    if (vec.x == 0.f && vec.y == 0.f)
        return vec;
    float len = sqrtf(vec.x*vec.x + vec.y*vec.y);
    return (Vector2){ vec.x / len, vec.y / len };
}

#endif
