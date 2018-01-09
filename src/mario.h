#ifndef MARIO_H
#define MARIO_H

#include <stdint.h>
#include <math.h>

#include "level.h"
#include "sprites.h"

extern int16_t odx; // old dx from jump start

extern void checkMarioGround();

extern void moveOutOfGround();

extern int16_t popOut(uint32_t x1, uint16_t w1, uint32_t x2, uint16_t w2);

#endif