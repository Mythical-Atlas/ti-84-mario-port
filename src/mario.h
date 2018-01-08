#ifndef MARIO_H
#define MARIO_H

#include <stdint.h>
#include <math.h>

#include "level.h"
#include "sprites.h"

extern int16_t odx; // old dx from jump start

extern void checkMarioGround();

extern void moveOutOfGround();

#endif