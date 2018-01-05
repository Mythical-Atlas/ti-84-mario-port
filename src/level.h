#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

#include <graphx.h>

// in order for a child of main.c (as in, it is included in main.c) to be able to use a variable that main.c needs to use, it must be declared here as an extern and redeclared in the child file

extern bool up; // whether or not the up arrow is being pressed
extern bool left; // whether or not the left arrow is being pressed
extern bool right; // whether or not the right arrow is being pressed

extern uint16_t width; // level width
extern uint16_t height; // level height

extern bool direction; // direction mario's facing (0 = right, 1 = left)

extern uint16_t x; // screen x

extern uint32_t mx; // mario's x position (1/256 of a pixel)
extern uint32_t my; // mario's y position (1/256 of a pixel)
extern int16_t dx; // delta x (x speed)
extern int16_t dy; // delta y (y speed)

extern gfx_tilemap_t tilemap;

extern uint16_t blocks;
extern uint16_t xBlocks[80];
extern uint16_t yBlocks[80];

extern void drawMario();
extern void updateMario();

extern bool checkCollision(uint16_t xy1, uint16_t wh1, uint16_t xy2, uint16_t wh2);

#endif