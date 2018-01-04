#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>

#include <graphx.h>

extern uint16_t width; // level width
extern uint16_t height; // level height

extern uint16_t x; // screen x
extern uint32_t mx; // mario's x position (1/256 of a pixel)
extern uint32_t my; // mario's y position (1/256 of a pixel)

extern gfx_tilemap_t tilemap;

extern uint16_t blocks;
extern uint16_t xBlocks[80];
extern uint16_t yBlocks[80];

#endif