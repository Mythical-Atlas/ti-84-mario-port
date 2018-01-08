#ifndef MARIO_H
#define MARIO_H

#include <stdint.h>
#include <math.h>

#include "level.h"
#include "sprites.h"

extern void checkMarioGround();

extern int16_t findGroundDistance(uint16_t i);

extern void moveOutOfGround();

#endif